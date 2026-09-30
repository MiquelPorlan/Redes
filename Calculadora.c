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
    if (strcmp(operacion, "+") == 0) {
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

void separar_cadena(char *cadena){
// separar la cadena en los dos primeros numeros y la operacion.

}

