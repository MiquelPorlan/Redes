#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_GRADO 5
#define BUFFER_SIZE 1024

int recuperarpolinomios(char *cadena, float *x, int *G1, float *h, int *G2){
    char *token = strtok(cadena, " \t\r\n"); // obtenemos el primer token

    if (token != NULL) { //G1 y G2
        *G1 = atoi(token); // Convertimos el primer token a entero
        token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
       
        *G2 = atoi(token); // Convertimos el segundo token a entero
        token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
    }
    for (int i = 0; i <= *G1; i++) { // Recorrer hasta G1 para llenar el array x
        if (token != NULL) {
            x[i] = atof(token); // Convertimos el token a float y lo almacenamos en x
            token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
        }
    }

    for (int i = 0; i <= *G2; i++) { // Recorrer hasta G2 para llenar el array h
        if (token != NULL) {
            h[i] = atof(token); // Convertimos el token a float y lo almacenamos en h
            token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
        }
    }
    return 0;
}

void convolucionDiscreta(float x[],int G1,float h[], int G2,float y[],int *G3){
    //sumatorio
    // x[k]*h[n-k]
    int N = G1 + 1; // Número de elementos de x (grado + 1)
    int M = G2 + 1; // Número de elementos de h (grado + 1)
    *G3 = N+M-1;
    for (int i = 0; i < *G3; i++) {
        y[i] = 0.0f;
    }
    for (int n=0 ; n<*G3 ; n++){
        for (int k=0 ; k<N ; k++){
            if(n - k >= 0 && n - k < M){
                y[n]+=x[k]*h[n-k];
            }
        }
    }
}

void productoPolinomios(char *cadena, float *y,int *G3){
    int G1, G2;
	float x[MAX_GRADO], h[MAX_GRADO];
    recuperarpolinomios(cadena, x, &G1, h, &G2);
    convolucionDiscreta(x,G1,h,G2,y,G3);
}

/*void productoPolinomios(char *cadena, float *y, int *G3){
    int G1, G2;
    float x[MAX_GRADO] = {0}, h[MAX_GRADO] = {0};
    
    // Creamos una copia local para que strtok no destruya la cadena original
    char cadena_copia[BUFFER_SIZE];
    strcpy(cadena_copia, cadena);
    
    recuperarpolinomios(cadena_copia, x, &G1, h, &G2);
    convolucionDiscreta(x, G1, h, G2, y, G3);
}*/

int obtenerPolinomio(float *p, int *grado) {
    scanf("%d",grado);
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

void formatearPolinomios(float *x, int G1, float *h, int G2, char *cadena) {
	sprintf(cadena,"%d %d", G1, G2);
	for(int i=0;i<=G1;i++){
		sprintf(cadena + strlen(cadena), " %g", x[i]);
	}
	for(int i=0;i<=G2;i++){
		sprintf(cadena + strlen(cadena), " %g", h[i]);
	}
}

/*int main(){
    int G1, G2, G3;
    float x[MAX_GRADO] = {0}, h[MAX_GRADO]= {0},y[2*MAX_GRADO-1]= {0};
    char cadena [BUFFER_SIZE];
    printf("Ingrese el grado del primer polinomio (maximo %d): ", MAX_GRADO-1);
    obtenerPolinomio(x, &G1);
    printf("Ingrese el grado del segundo polinomio (maximo %d): ", MAX_GRADO-1);
    obtenerPolinomio(h, &G2);

    formatearPolinomios(x,G1,h,G2,cadena);


    printf("Cadena formateada: !%s!\n", cadena);
    recuperarpolinomios(cadena,x,&G1,h,&G2);
    printf("!%i ",G1);
    printf("%i",G2);    

    for (int i=0;i<G1+1;i++){
        printf(" %g",x[i]);
    }
    for (int i=0;i<G2+1;i++){
        printf(" %g",h[i]);
    }

    productoPolinomios(cadena,y,&G3);
    printf("! %d\n",G3);
    for(int i=0 ; i<G3 ; i++){
        printf("%g",y[i]);
    }
}*/

int main(){
    int G1, G2, G3;
    float x[MAX_GRADO] = {0}, h[MAX_GRADO] = {0}, y[2 * MAX_GRADO - 1] = {0};
    char cadena[BUFFER_SIZE];
    
    printf("Ingrese el grado del primer polinomio (maximo %d): ", MAX_GRADO - 1);
    if (obtenerPolinomio(x, &G1) == -1) return 1;
    
    printf("Ingrese el grado del segundo polinomio (maximo %d): ", MAX_GRADO - 1);
    if (obtenerPolinomio(h, &G2) == -1) return 1;

    // 1. Formateamos
    formatearPolinomios(x, G1, h, G2, cadena);  
    printf("\n[DEBUG] Cadena generada: %s\n", cadena);

    // 2. Limpiamos las variables para verificar que se recuperan bien
    int G1_rec = 0, G2_rec = 0;
    float x_rec[MAX_GRADO] = {0}, h_rec[MAX_GRADO] = {0};
    
    
    printf("[DEBUG] G1 recuperado: %d, G2 recuperado: %d\n", G1_rec, G2_rec);

    // 3. Calculamos la convolución
    productoPolinomios(cadena, y, &G3);
    
    printf("[DEBUG] G3 calculado (número de elementos): %d\n", G3);
    printf("Coeficientes resultantes (y):");
    for(int i = 0; i < G3; i++){
        printf(" y[%d]=%g", i, y[i]);
    }
    printf("\n");
    
    return 0;
}




