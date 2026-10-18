// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Calculadora.c"

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024

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

// Función opcional para limpiar procesos huérfanos (sigaction / waitpid)
void sigchld_handler(int s) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[]) {
    int sock, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int port;
    char *endptr;

    if (argc == 1) {
        port = DEFAULT_PORT;
        printf("Cap port indicat. Utilitzant valor per defecte: %d\n", port);
    }
    else if (argc == 2) {
        errno = 0;
        port = strtol(argv[1], &endptr, 10);
        if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
            fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Ús: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Crear el socket principal de escucha
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error en crear el socket");
        exit(EXIT_FAILURE);
    }

    // Evitar errores de "Address already in use" al reiniciar rápido
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error en fer el bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0) {
        perror("Error en escoltar");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Configurar para evitar procesos zombis al finalizar los hijos
    struct sa;
    // O simplemente usar signal para ignorar o limpiar procesos hijos finalizados
    signal(SIGCHLD, SIG_IGN); 

    printf("Servidor concurrent en funcionament, esperant connexions...\n");

    while (1) {
        // Acceptar connexió del client
        if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Error en acceptar la connexió");
            continue; // Si falla un accept, intentem amb el següent
        }

        printf("Nou client connectat. Creant procés fill...\n");

        //  APLICAR CONCURRENCIA MEDIANTE FORK 
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Error al crear el fork");
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        
        if (pid == 0) {
            //  PROCESO HIJO 
            // El hijo no necesita el socket principal de escucha
            close(sock); 

            char buffer[BUFFER_SIZE] = {0};
            char Out[BUFFER_SIZE] = "";
            uint16_t Menu;
            float resultat;
            char historial[3][BUFFER_SIZE/3] = {"","",""}; // Historial independiente por cliente (o compartido si usas memoria compartida, por simplicidad local al cliente)

            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                
                if (recv(new_socket, &Menu, sizeof(Menu), 0) <= 0) {
                    printf("Client desconnectat (PID: %d)\n", getpid());
                    break;
                }
                
                printf("Opcio rebuda del client (PID: %d): %i\n", getpid(), Menu);
                
                switch (Menu) {
                case 1: 
                    recv(new_socket, &buffer, sizeof(buffer), 0);
                    printf("Missatge rebut (PID: %d): %s\n", getpid(), buffer);
                    sprintf(Out, "%s. Mensaje recibido", buffer);
                    send(new_socket, Out, sizeof(Out), 0);
                    break;

                case 2: 
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
                    
                case 3: 
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
                    
                case 5: 
                    sprintf(Out, "Historial:\n1. %s\n2. %s\n3. %s", historial[0], historial[1], historial[2]);
                    send(new_socket, Out, sizeof(Out), 0);
                    break;
                    
                default:
                    break;
                }

                if ((strcmp(buffer, "EXIT") == 0 && Menu == 1) || Menu == 6) {
                    printf("Tancant connexió amb el client (PID: %d)...\n", getpid());
                    send(new_socket, "Connexió tancada\n", strlen("Connexió tancada\n"), 0);
                    break;
                }
            }

            close(new_socket); // Tancar el socket del client en el fill
            exit(EXIT_SUCCESS);      // Terminar el proceso hijo de forma segura
        } 
        else {
            // --- PROCESO PADRE ---
            // El padre cierra su copia del new_socket porque lo gestionará el hijo,
            // y vuelve inmediatamente a escuchar nuevas conexiones en accept().
            close(new_socket);
        }
    }

    close(sock);
    return 0;
}

