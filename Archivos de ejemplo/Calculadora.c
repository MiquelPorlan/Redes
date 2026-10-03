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

float Calculadora(char *cadena) {
    float num1, num2,res;
    char operacion;
/*
    printf("Introdueix l'operaco en el format correcte\nsuma: 'a + b'\nproducte: 'a * b'\nexponent: 'a ^ b'\narrel quadrada: 'v a': ");
    fgets(cadena, sizeof(cadena), stdin);
*/
    separar_cadena(cadena, &num1, &num2, &operacion);
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
    }

    return res;
}

/*int main() {
    int opcion=1;
    while (opcion!=0) {
        // menu de opciones
        printf("=== CALCULADORA ===\n");
        printf("1. Realitzar operacio\n");
        printf("0. Sortir\n");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1:
                getchar(); // Limpiar el buffer de entrada
                Calculadora();
                break;
            case 0:
                printf("Sortint de la calculadora...\n");
                break;
            default:
                printf("Opcio no valida. Intente de nou.\n");
        }    
    }
    return 0;
}
    */


