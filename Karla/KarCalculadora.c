// Codi de programació d'una calculadora de funcions matemàtiques bàsiques, en C. 
// Objectiu: al insertar una funció de primer o segon grau, la calculadora et donara els termes necesaris per graficarla.

// Caldra que l'usuari introdueixi la funció de forma explicita, retornat la pendent, el tall ammb els eixos i dos punts de la funció per poder graficarla. 

#include <stdio.h>   // fgets() i printf()
#include <stdlib.h>  // strtof()
#include <string.h>  // strchr()
#include <math.h>


int main() { 

char funcio[100];
char primer_grau[100];
char segon_grau[100];


printf("Voldras introduir una funció de primer o segon grau? (1/2): ");
fgets(funcio, sizeof(funcio), stdin);
funcio[strcspn(funcio, "\n")] = 0; // Eliminar el salt de línia al final

if (funcio[0] == '1') {
       
    printf("Introdueix la funció de primer grau en la forma explicita (ax+b, sense espais i amb termes a i b visibles): ");
    fgets(primer_grau, sizeof(primer_grau), stdin);
    primer_grau[strcspn(primer_grau, "\n")] = '\0'; // Eliminar el salt de línia al final

    // Buscar la x
    char *posicio_x = strchr(primer_grau, 'x');

    if (posicio_x == NULL) {
        printf("No s'ha trobat la x.\n");

    } else {
        // Mostrar el que hi ha abans de la x
        printf("Terme A: %.*s\n",
        (int)(posicio_x - primer_grau),
        primer_grau);

        // Mostrar des de la x fins al final
        printf("Terme B: %s\n", posicio_x + 1);

        float pendent = strtof(primer_grau, NULL);

        // Calcular el tall amb l'eix y (quan x = 0)
        float tall_y = pendent * 0 + strtof(posicio_x + 1, NULL); // Convertir el que hi ha després de la x a float
        printf("Tall amb l'eix y: %g\n", tall_y);

        // Calcular el tall amb l'eix x (quan y = 0)
        if (pendent != 0) {
            float tall_x = -strtof(posicio_x + 1, NULL) / pendent; // Convertir el que hi ha després de la x a float
            printf("Tall amb l'eix x: %g\n", tall_x);
        } else {
            printf("La funció és constant, no hi ha tall amb l'eix x.\n");
        }  

        // Calcular dos punts de la funció per poder graficarla
        float x1 = 1.0; // Primer punt
        float y1 = pendent * x1 + strtof(posicio_x + 1, NULL);
        printf("Punt 1: (%g, %g)\n", x1, y1);  

        float x2 = 2.0; // Segon punt
        float y2 = pendent * x2 + strtof(posicio_x + 1, NULL);
        printf("Punt 2: (%g, %g)\n", x2, y2);

        while (1) {
            printf("Tornar al menú principal: 1.\n");
            printf("Introdueix una x de la funció per calcular la y: 3.\n");
            printf("Introdueix una y de la funció per calcular la x: 4.\n");

            fgets(funcio, sizeof(funcio), stdin);

            if (funcio[0] == '3') {
                float x;
                printf("Introdueix el valor de x: ");
                scanf("%f", &x);

                float y = pendent * x + strtof(posicio_x + 1, NULL);
                printf("Per x = %g, y = %g\n", x, y);

            } else if (funcio[0] == '4') {
                float y;
                printf("Introdueix el valor de y: ");
                scanf("%f", &y);

                if (pendent != 0) {
                    float x = (y - strtof(posicio_x + 1, NULL)) / pendent;
                    printf("Per y = %g, x = %g\n", y, x);

                } else {
                    printf("La funció és constant, no es pot calcular x per un valor de y diferent del tall amb l'eix y.\n");
                }

            } else if (funcio[0] == '1') {
                printf("Sortint del programa.\n");
                return 0;

            } else {
                printf("Opció no vàlida. Si us plau, introdueix 3 o 4.\n");
            }
        }
    }

    } else if (funcio[0] == '2') {

    printf("Introdueix la funció de segon grau en la forma explicita (ax^2+bx+c, sense espais i amb termes a, b i c visibles): ");
    fgets(segon_grau, sizeof(segon_grau), stdin);
    segon_grau[strcspn(segon_grau, "\n")] = '\0';

    
    char *posicio_x2 = strstr(segon_grau, "x^2"); // strstr() busca mes d'un caracter

    if (posicio_x2 == NULL) {
        printf("No s'ha trobat x^2.\n");
    } 
    else {
        char *posicio_x = strchr(posicio_x2 + 3, 'x');

        if (posicio_x == NULL) {
            printf("No s'ha trobat la segona x.\n");
        } 
        
        else {
            float a = strtof(segon_grau, NULL);
            float b = strtof(posicio_x2 + 3, NULL);
            float c = strtof(posicio_x + 1, NULL);

            printf("Terme A: %g\n", a);
            printf("Terme B: %g\n", b);
            printf("Terme C: %g\n", c); 

            // Calcular el tall amb l'eix y (quan x = 0)
            float tall_y = c;
            printf("Tall amb l'eix y: (0,%g)\n", tall_y);

            // Calcular el tall amb l'eix x (quan y = 0)
            float arrel = b*b - 4*a*c;
            if (arrel > 0) {
                float tall_x1 = (-b + sqrt(arrel)) / (2*a);
                float tall_x2 = (-b - sqrt(arrel)) / (2*a);
                printf("Tall amb l'eix x: (%g,0) i (%g,0)\n", tall_x1, tall_x2);
            } 
            else if (arrel == 0) {
                float tall_x = -b / (2*a);
                printf("Tall amb l'eix x: (%g,0)\n", tall_x);
            } 
            else {
                printf("No hi ha tall amb l'eix x.\n");
            }
        
            // Calcular vertex de la funció
            float vertex_x = -b / (2*a);
            float vertex_y = a*vertex_x*vertex_x + b*vertex_x + c;
            printf("Vertex: (%g, %g)\n", vertex_x, vertex_y);

            //Calcular dos punts de la funció per poder graficarla
            float x1 = vertex_x - 1.0; // Primer punt
            float y1 = a*x1*x1 + b*x1 + c;
            printf("Punt 1: (%g, %g)\n", x1, y1);

            float x2 = vertex_x + 1.0; // Segon punt
            float y2 = a*x2*x2 + b*x2 + c;
            printf("Punt 2: (%g, %g)\n", x2, y2);
        } 
    }
}
    else {
        printf("Opció no vàlida. Si us plau, introdueix 1 o 2.\n");
    }
return 0;
}