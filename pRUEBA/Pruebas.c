/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
    int num1, num2;
    char operacion;
    char cadena[]="23 + 43"; // Cadena de entrada
    char *datos[3];    
    int i = 0; // iniciar el contador de tokens
    char *token = strtok(cadena, " "); // Usamos el espacio como delimitadores

    while (token != NULL) {
        datos[i] = token; // Guardamos el token en el arreglo
        printf("Token %d: %s\n", i, token); // Imprimimos el token
        token = strtok(NULL, " "); // Siguientes llamadas con NULL, NULL hace que se use la misma direccion que antes
        i++;
    }
    printf("Los tokens son: ");
    num1 = atoi(datos[0]);
    operacion = datos[1][0];
    num2 = atoi(datos[2]);

    printf("%d %c %d\n", num1, operacion, num2);
return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void separar_cadena(char *cadena,  float *num1, float *num2, char *operacion){
    char *datos[3];    
    int i = 0; // iniciar el contador de tokens
    char *token = strtok(cadena, " "); // obtenemos el primer token

    while (token != NULL && i < 3) { // i < 3 para evitar desbordamiento 
        datos[i] = token; // Guardamos el token en el arreglo
        token = strtok(NULL, " "); // Siguientes llamadas con NULL, NULL hace que se use la misma direccion que antes
        i++;
    }
    if(i == 3) {
        printf("Error: La cadena no contiene suficientes tokens.\n");
        exit(1);
    }
    *num1 = atof(datos[0]);
    *operacion = datos[1][0];
    *num2 = atof(datos[2]);

}

int main() {
    float num1, num2;
    char operacion;
    char cadena[100]; // Cadena de entrada

    printf("Ingrese la operación (ej. '23 + 43'): ");
    fgets(cadena, sizeof(cadena), stdin);

    separar_cadena(cadena, &num1, &num2, &operacion);
    printf("%f %c %f\n", num1, operacion, num2);

    return 0;
}