#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int separar_cadena(char *cadena,  float *num1, float *num2, char *operacion){
    char *datos[3];    
    int i = 0; // iniciar el contador de tokens
    char *token = strtok(cadena, " "); // obtenemos el primer token

    while (token != NULL && i < 3 ) { // i < 3 para evitar desbordamiento 
        datos[i] = token; // Guardamos el token en el arreglo
        token = strtok(NULL, " "); // Siguientes llamadas con NULL, NULL hace que se use la misma direccion que antes
        i++;
    }
    if (i == 3) {
        *num1 = atof(datos[0]);
        *operacion = datos[1][0];
        *num2 = atof(datos[2]);
    }else if (i == 2){
        *num1 = atof(datos[1]);
        *operacion = datos[0][0];
        *num2 = atof(datos[1]);
    }else {
        printf("Error, vuelve a enviar la operacion en el formato correcto\n");
        return 0;
    }
    return 1;
}

int main() {
    float num1, num2;
    char operacion;
    char cadena[100]; // Cadena de entrada
   
   printf("Ingrese la operación en el siguiente formato:\nsuma: 'a + b'\nproducto: 'a * b'\nexponente: 'a ^ b'\nraíz cuadrada: 'v a': ");
    do
    {
    fgets(cadena, sizeof(cadena), stdin);
    } while (!separar_cadena(cadena, &num1, &num2, &operacion));
    printf("%.2f %c %.2f\n", num1, operacion, num2);

    return 0;
}