#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define DEFAULT_PORT 8080
#define DEFAULT_DOMAIN "localhost"
#define BUFFER_SIZE 1024

	typedef struct {
		uint16_t Menu;
		char cadena[BUFFER_SIZE];
	} Data;

void cerrarServidor(int sock, Data *datos) {
	
	strcpy(datos->cadena, "EXIT"); // Preparar mensaje de cierre de servidor
	send(sock, datos, sizeof(*datos), 0);  // Enviar missatge de tancament al servidor
	close(sock);
	printf("Connexió tancada. Sortint del client...\n");
}

void limpiar(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}


int main(int argc, char *argv[]) {
	int sock = 0;
	struct sockaddr_in serv_addr;

	Data datos = {0};	

	char buffer[BUFFER_SIZE] = {0};

	char *domini;
	int port;
	char *endptr;

	if (argc == 1) {
		// Cap argument -> valors per defecte
		domini = DEFAULT_DOMAIN;
		port = DEFAULT_PORT;
		printf("Cap argument indicat. Utilitzant valors per defecte: %s %d\n", domini, port);
	} 
	else if (argc == 3) {
		domini = argv[1];

		errno = 0;
		port = strtol(argv[2], &endptr, 10);

		// Comprovacions de validesa
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	} 
	else {
		fprintf(stderr, "Ús: %s [nom_de_domini port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Crear el socket
	// Afegiu comentari explicant els arguments, i de quines altres opcions hi ha
	// per al segon d'ells (ara SOCK_STREAM)
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\nError en crear el socket\n");
		return -1;
	}

	// Afegiu comentari sobre què són aquests 3 paràmetres,
	// per què s'utilitzen els valors que hi ha,
	// i per què s'utilitza htons()

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_pton(AF_INET, domini, &serv_addr.sin_addr);	// Convertir adreça IPv4 a binari
   

	// Connectar amb el servidor
	// Afegiu comentari explicant els arguments
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnexio fallida\n");
		return -1;
	}

	printf("Connectat al servidor. Podeu començar a enviar missatges.\n");

	while (1) {
		memset(buffer, 0, sizeof(buffer)); // Netegem el buffer
		// Menú principal
		// Cal que implementeu un petit servei remot amb almenys 4 funcionalitats noves
		// Definiu vosaltres mateixos les dades a enviar (demanar en el client, i analitzar al servidor) i rebre
		// Si per donar més sentit al servei cal alguna opció més, la podeu afegir
		// Deixeu l'opció 1 com a Enviar missatge i l'última per Sortir

		printf("Menú principal:\n");
		printf("1. Enviar missatge\n");
		printf("2. Calcular operacion\n");
		printf("3. historial\n");
		printf("4. Opció 4\n");
		printf("5. Opció 5\n");
		printf("6. Sortir\n");
		printf("Opció: ");

		scanf("%hd", &datos.Menu);
		limpiar();
		while (getchar() != '\n');  // buida el buffer fins al salt de línia

		switch (datos.Menu)
		{
		case 1:
            // Llegir missatge del client
			printf("Introdueix el missatge a enviar ('EXIT' per tancar el servidor i sortir): ");
			fgets(datos.cadena, BUFFER_SIZE, stdin);
			datos.cadena[strcspn(datos.cadena, "\n")] = '\0';  // Eliminar \n final
			if (strcmp(datos.cadena, "EXIT") == 0) {
				cerrarServidor(sock, &datos);
				return 0;
			}
			break;

		case 2:
			printf("Introdueix l'operació a calcular \n(format: 'a + b', 'a * b', 'a ^ b', 'v a'): ");
			fgets(datos.cadena, BUFFER_SIZE, stdin);
			datos.cadena[strcspn(datos.cadena, "\n")] = '\0';  // Eliminar \n final
			break;
		case 3:

			break;

		case 4:
			// Implementar Opció 4
			break;

		case 5:
			datos.Menu = 5;
			strcpy(datos.cadena, ""); // No cal cap dada addicional per a l'historial
			break;

        case 6:
            // Preparar mensaje de cierre de servidor
			cerrarServidor(sock, &datos);
			return 0;
            break;
		default:
			printf("Opció invàlida\n");
			break;
		}
		if (datos.Menu >= 1 && datos.Menu <= 6) {
			send(sock, &datos, sizeof(datos), 0);  // Enviar dades al servidor
			recv(sock, buffer, sizeof(buffer), 0);  // Rebre resposta del servidor
			printf("Resposta del servidor: %s\n", buffer);
		}

	
	}

	close(sock);
	return 0;
}