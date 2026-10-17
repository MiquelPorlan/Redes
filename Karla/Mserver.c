// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Calculadora.c"

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 1024

	typedef struct{
		uint16_t Menu;
		char cadena[BUFFER_SIZE];
	} Data;


int main(int argc, char *argv[]) {
	int sock, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	char buffer[BUFFER_SIZE] = {0};
	char Out[BUFFER_SIZE] = "";

	Data datos;
	float resultat;
	char historial[3][BUFFER_SIZE/3] = {"","",""}; // Historial de las últimas 3 operaciones

	int port;
	char *endptr;

	if (argc == 1) {
		// Cap argument -> port per defecte
		port = DEFAULT_PORT;
		printf("Cap port indicat. Utilitzant valor per defecte: %d\n", port);
	}
	else if (argc == 2) {
		errno = 0;
		port = strtol(argv[1], &endptr, 10);

		// Comprovacions
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "Ús: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	// Crear el socket
	// Afegiu comentari explicant els arguments
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Error en crear el socket");
		exit(EXIT_FAILURE);
	}


	// Afegiu comentari explicant què són aquests 3 paràmetres,
	// per què s'utilitzen htons() i htonl(),
	// i per què s'utilitzen els valors que hi ha

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);	// Per a INADDR_ANY no faria falta htonl(), però li posem per coherència
	address.sin_port = htons(port);

	// Enllaçar el socket al port especificat
	// Afegiu comentari explicant els arguments
	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Error en fer el bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	// Escoltar connexions entrants
	// Afegiu comentari explicant els arguments
	
	if (listen(sock, 3) < 0) {
		perror("Error en escoltar");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("Servidor en funcionament, esperant connexions...\n");

	while (1) {
		// Acceptar connexions de clients
		// Afegiu comentari explicant els arguments i per què hi ha i cal new_socket si ja tenim sock
		
		if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("Error en acceptar la connexió");
			close(sock);
			exit(EXIT_FAILURE);
		}

		 while (1) {
			memset(buffer, 0, BUFFER_SIZE);
			
			// Llegir missatge del client
			if (recv(new_socket, &datos, sizeof(datos), 0) <= 0) {
				printf("Client desconnectat (PID: %d)\n", getpid());
				break;
			}
			switch (datos.Menu)
			{
			case 1: //Recibir missatge del client i mostrar-lo per pantalla
				printf("Missatge rebut del client (PID: %d): %s\n", getpid(), datos.cadena);
				send(new_socket, "datos recibidos ", sizeof("datos recibidos "), 0);
				break;

			case 2: // recibir la operacion y devolver el resultado.
				printf("opcion %d = ", datos.Menu);
				printf("cadena %s\n", datos.cadena);
				sprintf(Out, "%s", datos.cadena);
				if (Calculadora(datos.cadena, &resultat)) {
					printf("%.2f (PID: %d)\n", resultat, getpid());
					sprintf(Out, "%s = %.2f", Out, resultat); //formatea el resultado para enviarlo como cadena
					send(new_socket, Out, sizeof(Out), 0);
				} else {
					send(new_socket, "Error en la operación\n", strlen("Error en la operación\n"), 0);
					sprintf(Out, "%s = Error", Out);
				}
				// Almacenar la operación en el historial
				for (int i = 2; i > 0; i--) {
					strcpy(historial[i], historial[i - 1]);
				}	
				strcpy(historial[0], Out);
				break;	
			
			case 3: // enviar el historial de operaciones
				sprintf(Out, "Historial:\n1. %s\n2. %s\n3. %s", historial[0], historial[1], historial[2]);
				send(new_socket, Out, sizeof(Out), 0);
				break;

				
			default:
				break;
			}


			
			
			// A continuació hi ha el codi corresponent a l'opció d'Enviar missatge, amb el retorn d'una cadena fixa,
			// i la de Sortida de la connexió/bucle si és el cas.
			// Modifiqueu la cadena de retorn per tal que sigui la que diu l'enunciat.

			

			// Comprovar si el client vol tancar la connexió
			if (strcmp(datos.cadena, "EXIT") == 0 && (datos.Menu == 1 || datos.Menu == 6)) {
				printf("Tancant connexió amb el client (PID: %d)...\n", getpid());
				send(new_socket, "Connexió tancada\n", strlen("Connexió tancada\n"), 0);
				break;	// Sortir del bucle
			}
		}

		close(new_socket); // Tancar la connexió amb el client
	}

	close(sock);
	return 0;
}
