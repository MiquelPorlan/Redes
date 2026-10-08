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
        }
        *resultat = res;
        return 1;
    }else {
        return 0; // Error al separar la cadena
    }


}


