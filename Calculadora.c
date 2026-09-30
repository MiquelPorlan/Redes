#include <stdio.h>
#include <string.h>
#include <math.h>

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

int detectar_operacion(char *operacion){
    if        (strcmp(operacion, "+") == 0) {
        return 1;
    } else if (strcmp(operacion, "*") == 0) {
        return 2;
    } else if (strcmp(operacion, "^") == 0) {
        return 3;
    } else if (strcmp(operacion, "v") == 0) {
        return 4;
    } else {
        return -1; // Operación no reconocida
    }
}

void separar_cadena(char *cadena,  float *num1, float *num2, char *operacion){
    char *datos[3];    
    int i = 0; // iniciar el contador de tokens
    char *token = strtok(cadena, " "); // obtenemos el primer token

    while (token != NULL && i < 3) { // i < 3 para evitar desbordamiento 
        datos[i] = token; // Guardamos el token en el arreglo
        token = strtok(NULL, " "); // Siguientes llamadas con NULL, NULL hace que se use la misma direccion que antes
        i++;
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


