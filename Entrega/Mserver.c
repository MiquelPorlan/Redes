/* b6 
Miquel Porlan
Karla suquia
Eduard Muñoz
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>


#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024
#define MAX_GRADO 5


void historialOperaciones(char historial[3][BUFFER_SIZE/3], char *Out) ;
int recibirPolinomio(int sock, float *x, uint16_t grado);
float suma(float a, float b);
float producto(float a, float b);
float exponente(float base, int exponente);
float raiz_cuadrada(float numero);
int detectar_operacion(char operacion);
int separar_cadena(char *cadena,  float *num1, float *num2, char *operacion);
int Calculadora(char *cadena, float *resultat);
void convolucionDiscreta(float x[],int G1,float h[], int G2,float y[],uint16_t *G3);
float CalcularTallEixX(float a, float b, float c, float *tall_x1, float *tall_x2);


int main(int argc, char *argv[]) {
    int sock, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int port;
    char *endptr;

    if (argc == 1) { // no se han indicado valores adicionales al ejecutar el codigo, por lo que se usan valolores por defecto.
        port = DEFAULT_PORT;
        printf("Cap port indicat. Utilitzant valor per defecte: %d\n", port);
    }
    else if (argc == 2) { // el programa ha recibido exactamente 2 argumentos, el propio progrma cuenta como un argumento.
        errno = 0;
        port = strtol(argv[1], &endptr, 10); // port = segundo argumento que recibe el main, guarda la direccion del primer caracter no numerico en *endptr
        if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) { // si el puerto esta fuera de rango o contiene caracteres no numericos genera un mensaje de error y cierra el progama.
            fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Ús: %s [port]\n", argv[0]); // si contiene mas argumentos que el del puerto da un error: "Us |nombre del programa| [port]""
        exit(EXIT_FAILURE);
    }

    // Crear el socket principal de escucha
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) { // crea la conexion y le asocia el identificadoir a la variable sock
        perror("Error en crear el socket");                 // si al crear el sock daa error envia un mensaje de error y cierra el programa
        exit(EXIT_FAILURE);
    }

    // Evitar errores de "Address already in use" al reiniciar rápido
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // la funcion permite modificar paramtros de los sock, el SO suele mantener los puertos resrvados un tiempo, por lo que al usar el SO_REUSEADDR permite "saltarse" este bloqueo

    address.sin_family = AF_INET; // define que la familia de direcciones es IPv4
    address.sin_addr.s_addr = htonl(INADDR_ANY); // configura la direccio IP a traves de la que se va escuchar las conexiones entrantes
    address.sin_port = htons(port); // asigna el puerto por el que se va a escucahr las peticiones

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) { // asocia la informacion de conexion antes declarada al socket que se acaba de abrir para recibir la conexiones. 
        perror("Error en fer el bind");                                 // si falla manda un mensaje de error, cierra el socket y sale del programa
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0) { // prepara el socket para recibir las solicitudes de conexion
        perror("Error en escoltar");    // si falla manda un mensaje de error y cierra el sock y el programa
        close(sock);
        exit(EXIT_FAILURE);
    }

    //usar signal para ignorar o limpiar procesos hijos finalizados
    signal(SIGCHLD, SIG_IGN); 

    printf("Servidor concurrent en funcionament, esperant connexions...\n");

    while (1) {
        // Acceptar connexió del client
        if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) { // acepta la conexion con un cliente y le asigna otro socket para realizar la comuniacaiones dejando dispolible el sock original
            perror("Error en acceptar la connexió"); 
            continue; // Si falla un accept, intentem amb el següent
        }

        printf("Nou client connectat. Creant procés fill...\n");

        //  APLICAR CONCURRENCIA MEDIANTE FORK 
        pid_t pid = fork(); // crea el proceso hijo que utilizara el socket del accept para ejecutar sus operaciones
        
        if (pid < 0) {
            perror("Error al crear el fork");
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        
        if (pid == 0) { // solo el proceso hijo entra aqui al tener pid = 0
            //  PROCESO HIJO 
            // El hijo no necesita el socket principal de escucha
            close(sock); 

            char buffer[BUFFER_SIZE] = {0};
            char Out[BUFFER_SIZE] = "";
            uint16_t Menu;
            float resultat;
            char historial[3][BUFFER_SIZE/3] = {"","",""}; 

            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                
                if (recv(new_socket, &Menu, sizeof(Menu), 0) <= 0) { // recibe del cliente la opcion para la que se tine que preparar el servidor
                    printf("Client desconnectat (PID: %d)\n", getpid());
                    break;
                }
                
                printf("Opcio rebuda del client (PID: %d): %i\n", getpid(), Menu);
                
                switch (Menu) {
                case 1: // recibe el mensaje y envia mensaje de recepcion al cliente
                    recv(new_socket, &buffer, sizeof(buffer), 0);
                    printf("Missatge rebut (PID: %d): %s\n", getpid(), buffer);
                    sprintf(Out, "%s. Mensaje recibido", buffer);
                    send(new_socket, Out, sizeof(Out), 0);
                    break;

                case 2: // recibe la cadena de operacion del servidor, la separa y realiza la operacion, ademas de guardar el resultado en un historial
                    recv(new_socket, &buffer, sizeof(buffer), 0);
                    sprintf(Out, "%s", buffer);
                    if (Calculadora(buffer, &resultat)) {
                        printf("Resultat: %.2f (PID: %d)\n", resultat, getpid());
                        sprintf(Out, "%s = %.2f", Out, resultat);
                        send(new_socket, Out, sizeof(Out), 0);
                    } else {
                        send(new_socket, "Error en la operación\n", strlen("Error en la operación\n"), 0);
                        sprintf(Out, "%s = Error", Out);
                    }
                    historialOperaciones(historial, Out);
                    break;
                    
                case 3: // recibe del  cliente tanto los polinomios como su grado para tratarlos
                    {
                        uint16_t G1, G2, G3;
                        float x[MAX_GRADO]={0}, h[MAX_GRADO]={0}, sol[2*MAX_GRADO-1]={0};
                        Out[0]='\0';
                        recv(new_socket, &G1, sizeof(G1), 0);
                        recibirPolinomio(new_socket, x, G1);
                        recv(new_socket, &G2, sizeof(G2), 0);
                        recibirPolinomio(new_socket, h, G2);

                        convolucionDiscreta(x, G1, h, G2, sol, &G3);
                        sprintf(Out + strlen(Out), "%g ", sol[0]);
                        for(int i=1; i<G3; i++){
                            sprintf(Out + strlen(Out), "+ %g*(x^%d)", sol[i], i);
                        }
                        
                        send(new_socket, Out, sizeof(Out), 0);
                        historialOperaciones(historial, Out);
                    }
                    break;

                case 4: // recibe una funcion de hasta segundo grado en un formato separado por comas, y encuentra los cortes con los ejes
                    int grado;
                    float a,b,c;
                    float x1,x2;
                    int erro;

                    recv(new_socket, buffer, sizeof(buffer), 0);
                    sscanf(buffer,"%d;%f;%f;%f",&grado,&a,&b,&c);
                    if (!CalcularTallEixX(a,b,c,&x1,&x2)){
                        if (grado == 2){
                            sprintf(Out, "puntos de corte: \nx1:%.2g\nx2:%.2g\ny:%.2g", x1, x2, c);
                        }    
                        if (grado == 1){
                           sprintf(Out, "puntos de corte: \nx1:%.2g\ny:%.2g", x1, c);
                        }    
                    }else{
                        sprintf(Out, "puntos de corte: \ny:%.2g", c);
                    }
                    

                    send(new_socket, Out, sizeof(Out), 0);
                    break;

                case 5: // envia el historial de las operaciones 2 y 3
                    sprintf(Out, "Historial:\n1. %s\n2. %s\n3. %s", historial[0], historial[1], historial[2]);
                    send(new_socket, Out, sizeof(Out), 0);
                    break;
                    
                default:
                    break;
                }

                if ((strcmp(buffer, "EXIT") == 0 && Menu == 1) || Menu == 6) { // cierra el servidor 
                    printf("Tancant connexió amb el client (PID: %d)...\n", getpid());
                    send(new_socket, "Connexió tancada\n", strlen("Connexió tancada\n"), 0);
                    break;
                }
            }

            close(new_socket); // Tancar el socket del client en el fill
            exit(EXIT_SUCCESS);      // Terminar el proceso hijo de forma segura
        } 
        else {// el proceso padre tiene un PID con el identifiador del hijo, por los que PID != 0 y entra en esta seccion
            // --- PROCESO PADRE ---
            // El padre cierra su copia del new_socket porque lo gestionará el hijo,
            // y vuelve inmediatamente a escuchar nuevas conexiones en accept().
            close(new_socket);
        }
    }

    close(sock);
    return 0;
}

void historialOperaciones(char historial[3][BUFFER_SIZE/3], char *Out) {
    for (int i = 2; i > 0; i--) {
        strcpy(historial[i], historial[i - 1]);
    }   
    strcpy(historial[0], Out);
}

int recibirPolinomio(int sock, float *x, uint16_t grado){
    int size = (grado+1)*sizeof(float);
    recv(sock, x, size, 0);
}

float suma(float a, float b){
    return a + b;
}

float producto(float a, float b){
    return a * b;
}   

float exponente(float base, int exponente){
    float resultado = 1.0;
    for(int i = 0; i < exponente; i++){
        resultado *= base;
    }
    return resultado;
}

float raiz_cuadrada(float numero){
    float a=0;
    if (numero < 0) {
        a=-1;
    }
    else {
        a=sqrt(numero);
    }
    return a;
}

int detectar_operacion(char operacion){
    if        (operacion == '+') {      // Suma 
        return 1;
    } else if (operacion == '*') {      // Producto     
        return 2;
    } else if (operacion == '^') {      // Exponente    
        return 3;
    } else if (operacion == 'v') {      // Raíz cuadrada
        return 4;
    } else {
        return -1;                      // Operación no reconocida
    }
}

int separar_cadena(char *cadena,  float *num1, float *num2, char *operacion){
    char *datos[3];    
    int i = 0; // iniciar el contador de tokens
    char *token = strtok(cadena, " \t\r\n"); // obtenemos el primer token

    while (token != NULL && i < 3) { // i < 3 para evitar desbordamiento 
        datos[i] = token; // Guardamos el token en el arreglo
        token = strtok(NULL, " \t\r\n"); // Siguientes llamadas con NULL usan la misma cadena
        i++;
    }

    if (i == 3) {                       // separar los datos en num1, operacion y num2
        *num1 = atof(datos[0]);
        *operacion = datos[1][0];
        *num2 = atof(datos[2]);
    }else if (i == 2){
        *num1 = atof(datos[1]);
        *operacion = datos[0][0];
        *num2 = 0;
    }else {
        printf("Error, Torna a enviar les dades en el format correcte\n");
        return 0;
    }
    return 1;
}

int Calculadora(char *cadena, float *resultat) {
    float num1, num2,res;
    char operacion;

    if (separar_cadena(cadena, &num1, &num2, &operacion)) {
        switch (detectar_operacion(operacion)) {
            case 1:
                res = suma(num1, num2);
                break;
            case 2:
                res = producto(num1, num2);
                break;
            case 3:
                res = exponente(num1, (int)num2);
                break;
            case 4:
                if (!raiz_cuadrada(num1)) {
                    printf("Error: No es pot calcula l'arrel.\n");
                } else {
                    res = raiz_cuadrada(num1);
                }
                break;
            default:
                printf("Operacio no reconeguda.\n");
                return 0;
        }
        *resultat = res;
        return 1;
    }else {
        return 0; // Error al separar la cadena
    }


}

void convolucionDiscreta(float x[],int G1,float h[], int G2,float y[],uint16_t *G3){
        //sumatorio
        // x[k]*h[n-k]
        int N=G1+1;
        int M=G2+1;
        *G3=N+M-1;
    for (int n=0 ; n<*G3 ; n++){
        for (int k=0 ; k<N ; k++){
            if(n - k >= 0 && n - k < M){
                y[n]+=x[k]*h[n-k];
            }
        }
    }
}

float CalcularTallEixX(float a, float b, float c, float *tall_x1, float *tall_x2) {
    if (a == 0.0f) {
        if (b == 0.0f) {
            return 0;
        }

        *tall_x1 = -c / b;
        return 1;
    }

    float arrel = b*b - 4*a*c;

    if (arrel > 0) {
        *tall_x1 = (-b + sqrt(arrel)) / (2*a);
        *tall_x2 = (-b - sqrt(arrel)) / (2*a);
        return 2;
    } 
    else if (arrel == 0) {
        *tall_x1 = -b / (2*a);
        return 1;
    } 
    else {
        return 0;
    }
}

