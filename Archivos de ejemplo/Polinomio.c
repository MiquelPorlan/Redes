#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

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

int main(void) {
    float polinomio[5];
    int grado;
    printf("Ingrese el grado del polinomio: ");
    if (scanf("%d", &grado) != 1 || grado < 0) {
        printf("Error: el grado debe ser un entero no negativo.\n");
        return 1;
    }
    if (llenar_polinomio(polinomio, grado)) {
        printf("Polinomio ingresado correctamente.\n");

        for (int i = 0; i <= grado; i++) {
            if (polinomio[i] != 0) {
                printf("%g*x^%d ", polinomio[i], i);
            }
        }  
    } else {
        printf("Error al ingresar el polinomio.\n");
    }
    return 0;
}
    