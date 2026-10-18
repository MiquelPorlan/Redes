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
		printf("3. Producto de polinomios\n");
		printf("4. Opció 4\n");
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

		case 2:
			printf("Introdueix l'operació a calcular \n(format: 'a + b', 'a * b', 'a ^ b', 'v a'): ");
			fgets(cadena, BUFFER_SIZE, stdin);
			cadena[strcspn(cadena, "\n")] = '\0';  // Eliminar \n final
			send(sock, &cadena, strlen(cadena),0);
			break;
		case 3:
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

		case 4:
			// Implementar Opció 4
			break;

		case 5:
			Menu = 5;
			strcpy(cadena, ""); // No cal cap dada addicional per a l'historial
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