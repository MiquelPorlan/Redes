/* 2b 
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

#define DEFAULT_PORT 8080
#define DEFAULT_DOMAIN "localhost"
#define BUFFER_SIZE 1024
#define MAX_GRADO 5

void limpiar(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int obtenerPolinomio(float *p, uint16_t *grado) {
    scanf("%hd",grado);
    if(*grado<0 || *grado>=MAX_GRADO){
        printf("Error: grado invalido.\n");
        return -1;
    }
    for(int i=*grado;i>=0;i--){
        printf("Ingrese el coeficiente para x^%d: ",i);
        scanf("%f",&p[i]);
    }
    return 1;
}

void enviarPolinomio(int sock, float *polinomio, int Grado){
	int size = (Grado+1)*sizeof(float);
	send(sock, polinomio, size, 0);
}

int llegirCoficientsIGrau(float *a, float *b, float *c) {
       
    char segon_grau[100];

    int grau; // si existeix a, retornara 2. en cas contrari retornara 1. si no funcina 0.
    float termeA, termeB, termeC; // Variables per guardar els termes de la funció

    *a = 0.0f;
    *b = 0.0f;
    *c = 0.0f;

    printf("Introdueix una funció en la forma explicita (ax^2+bx+c, sense espais i amb termes a, b i c visibles): ");
    fgets(segon_grau, sizeof(segon_grau), stdin);
    segon_grau[strcspn(segon_grau, "\n")] = '\0'; //Neteja 

    char *posicio_x2 = strstr(segon_grau, "x^2"); // Busca fins x^2

    if (posicio_x2 == NULL) {
              
        char *posicio_x = strchr(segon_grau, 'x'); // Busca la x en el segon grau

        if (posicio_x == NULL) {
            grau = 0; // No s'ha trobat la x
        } 

        else {
            grau = 1; // S'ha trobat la x, és de primer grau

            termeB = strtof(segon_grau, NULL); // Converteix el que hi ha abans de la x a float
            termeC = strtof(posicio_x + 1, NULL); // Converteix el que hi ha després de la x a float
            
            *a = 0;
            *b = termeB;
            *c = termeC;
        }
    }

    else {
        char *posicio_x = strchr(posicio_x2 + 3, 'x');

        if (posicio_x == NULL) {
            grau = 0;
        } else {
            grau = (termeA == 0.0f) ? 1 : 2; // Si a és 0, és de primer grau; si no, és de segon grau

            termeA = strtof(segon_grau, NULL); // Converteix el que hi ha abans de x^2 a float
            termeB = strtof(posicio_x2 + 3, NULL); // Converteix el que hi ha després de x^2 a float
            termeC = strtof(posicio_x + 1, NULL); // Converteix el que hi ha després de la segona x a float

            *a = termeA;
            *b = termeB;
            *c = termeC;
        }
    }

    return grau;
}

int main(int argc, char *argv[]) {
	int sock = 0;
	struct sockaddr_in serv_addr;

	uint16_t Menu;
	char cadena[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];

	char *domini;
	int port;
	char *endptr;

	if (argc == 1) {
		// Cap argument -> valors per defecte
		domini = DEFAULT_DOMAIN;
		port = DEFAULT_PORT;
		printf("Cap argument indicat. Utilitzant valors per defecte: %s %d\n", domini, port);
	} 
	else if (argc == 3) {// se envian exactamente dos argumentos al ejecutar el programa
		domini = argv[1]; // el domini recive el primer argumento adicional

		errno = 0;
		port = strtol(argv[2], &endptr, 10);// el port recibe el valor del puerto y guarda en endprt el primer caracter no numerico

		// Comprovacions de validesa
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) { // genera un error si el puerto no esta en rango o tiene un caracter no numerico
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	} 
	else {
		fprintf(stderr, "Ús: %s [nom_de_domini port]\n", argv[0]); // si contiene mas de dos argumentos da error: "Us |nombre del programa| [nom_de_domini port]"
		exit(EXIT_FAILURE);
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // crea el socket para establecer la conexion con el cliente (IPv4 y TCP)
		printf("\nError en crear el socket\n"); // si da error se genera un mensaje de error y sale del programa
		return -1;
	}

	serv_addr.sin_family = AF_INET;// establece la  familia de direccion como IPv4
	serv_addr.sin_port = htons(port); // establece el puerto de conexion
	inet_pton(AF_INET, domini, &serv_addr.sin_addr);	// Convertir adreça IPv4 a binari
   
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { // se conecta con el servidor a traves de las IP y puerto configuradas anteriormente
		printf("\nConnexio fallida\n");// si no puede establecer la conexion da un mensaje de error
		return -1;
	}

	printf("Connectat al servidor. Podeu començar a enviar missatges.\n");

	while (1) {
		memset(buffer, 0, sizeof(buffer)); // Netegem el buffer

		printf("Menú principal:\n");
		printf("1. Enviar missatge\n");
		printf("2. Calcular operacion\n");
		printf("3. Producto de polinomios\n");
		printf("4. Funcion\n");
		printf("5. historial\n");
		printf("6. Sortir\n");
		printf("Opció: ");

		scanf("%hd", &Menu);
		send(sock, &Menu, sizeof(uint16_t), 0);
		limpiar();
		while (getchar() != '\n');  // buida el buffer fins al salt de línia

		switch (Menu){
		case 1:
            // Llegir missatge del client
			printf("Introdueix el missatge a enviar ('EXIT' per tancar el servidor i sortir): ");
			fgets(cadena, BUFFER_SIZE, stdin);
			cadena[strcspn(cadena, "\n")] = '\0';  // Eliminar \n final
			send(sock,&cadena,strlen(cadena),0);
			break;

		case 2:// lee la cadena ya en el formato y la envia al servidor tal cual 
			printf("Introdueix l'operació a calcular \n(format: 'a + b', 'a * b', 'a ^ b', 'v a'): ");
			fgets(cadena, BUFFER_SIZE, stdin);
			cadena[strcspn(cadena, "\n")] = '\0';  // Eliminar \n final
			send(sock, &cadena, strlen(cadena),0);
			break;

		case 3: // obtiene el polinomio y su grado para enviarlos al servidor
			uint16_t G1, G2;
			float x[MAX_GRADO], h[MAX_GRADO];
			
			printf("Ingrese el grado del primer polinomio (maximo %d): ", MAX_GRADO-1);
			if(!obtenerPolinomio(x,&G1)){
				return -1;
			}
			else {
				send(sock, &G1, sizeof(uint16_t),0);
				enviarPolinomio(sock, x, G1);
			}

    		printf("Ingrese el grado del segundo polinomio (maximo %d): ", MAX_GRADO-1);
			if(!obtenerPolinomio(h,&G2)){
				return -1;
			}else {
				send(sock, &G2, sizeof(uint16_t),0);
				enviarPolinomio(sock, h, G2);
			}

			break;

		case 4: // lee los coeficiente de primer grado y envia una cadena formateada al servidor
			int grado;
			float a, b, c;

			grado = llegirCoficientsIGrau(&a, &b, &c);
			sprintf(cadena,"%d;%f;%f;%f", grado, a, b, c);
			send(sock, &cadena, strlen(cadena), 0);
			
			break;
		case 5:	// opcion 5 y 6 no hacen nada ya que solo necesita enviar el menu
			break;
		case 6:
			break;

		default:
			printf("Opció invàlida\n");
			break;
		}

		if (Menu >= 1 && Menu <= 6){
			recv(sock, buffer, sizeof(buffer), 0);  // Rebre resposta del servidor
			printf("Resposta del servidor: %s\n", buffer);
			if ((strcmp(cadena, "EXIT") == 0 && Menu == 1) || Menu == 6 ){
				recv(sock, buffer, sizeof(buffer), 0);  // Rebre resposta del servidor
				printf("%s", buffer);
				break;
			}
		}
	}

	close(sock);
	return 0;
}