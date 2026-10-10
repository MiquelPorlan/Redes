#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

#define MAX_GRADO 10

typedef struct{
    int grado;
    float coeficientes[MAX_GRADO-1]; // Array para almacenar los coeficientes del polinomio
} Polinomio;

void limpiar(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int llenar_polinomio(float *p, int grado) {
    int i=0;
    for (i = grado; i >= 0; i--) {
        printf("Ingrese coeficiente para x^%d: ", i);
        if (scanf("%f", &p[i]) != 1) {
            printf("Error: coeficiente inválido.\n");
            return 0;
        }
    }
    return 1;
}

void convolucionDiscreta(float x[],int G1,float h[], int G2,float y[],int G3){
        //sumatorio
        // x[k]*h[n-k]
    for (int n=0 ; n<G3 ; n++){
        for (int k=0 ; k<G2 ; k++){
            if(n - k >= 0 && n - k < G2){
                y[n]+=x[k]*h[n-k];
            }
        }
    }
    for(int l=0 ; l<G3 ; l++){
        printf("%g\n",y[l]);
    }
}

int main(void) {

}


    