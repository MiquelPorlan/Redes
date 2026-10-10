#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_GRADO 10

/*convolucion
int main(){
    int n,k;
    float x[2]={1,1},h[2]={1,1},y[3]={0,0,0};
        //sumatorio
        // x[k]*h[n-k]
    for (n=0;n<3;n++){
        for (k=0;k<2;k++){
            if(n - k >= 0 && n - k < 2){
                y[n]+=x[k]*h[n-k];
            }
        }
    }
    for(int l=0 ; l<3 ; l++){
        printf("%g\n",y[l]);
    }
    return 0;
}
*/


int obtenerPolinomio(float *p, int *grado) {
    printf("Ingrese el grado del primer polinomio (maximo %d): ", MAX_GRADO-1);
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

void convolucionDiscreta(float x[],int longX,float h[], int longH,float y[],int *longY){
    *longY = longX + longH - 1;
    for(int i=0;i<*longY;i++){
        y[i]=0;
    }
        //sumatorio
        // x[k]*h[n-k]
    for (int n=0 ; n<*longY ; n++){
        for (int k=0 ; k<longH ; k++){
            if(n - k >= 0 && n - k < longH){
                y[n]+=x[k]*h[n-k];
            }
        }
    }
}
/*
int main(){
    float x[MAX_GRADO],h[MAX_GRADO],y[MAX_GRADO*2-1];
    int G1,G2,G3;

    if(!obtenerPolinomio(x,&G1)){
        printf("Error al obtener el primer polinomio.\n");
        return -1;
    }
    if(!obtenerPolinomio(h,&G2)){
        printf("Error al obtener el segundo polinomio.\n");
        return -1;
    }

    convolucionDiscreta(x,G1+1,h,G2+1,y,&G3);

    for(int i=0;i<=G1;i++){
        printf("x^%d=%g\n",i,x[i]);
    }
    for(int i=0;i<=G2;i++){
        printf("h^%d=%g\n",i,h[i]);
    }
    for(int i=0;i<G3;i++){
        printf("y^%d=%g\n",i,y[i]);
    }
}*/
/*
int imprimirPolinomios(){
    	int G1, G2, G3;
		float x[MAX_GRADO], h[MAX_GRADO], y[MAX_GRADO*2-1];
        char cadena[100]="";
		if(!obtenerPolinomio(x,&G1)){
			return -1;
		}
		if(!obtenerPolinomio(h,&G2)){
			return -1;
		}
        int posCadena=0;
		sprintf(cadena,"%d %d", G1, G2);
        posCadena=strlen(cadena);
		for(int i=0;i<=G1;i++){
			sprintf(cadena+posCadena+2*i, " %g", x[i]);
		}
        posCadena=strlen(cadena); 
		for(int i=0;i<=G2;i++){
			sprintf(cadena+posCadena+2*i, " %g", h[i]);
		}
        printf("string: %s\n", cadena);
}
*/


int main (){
char cadena[100]="2 1 1 2 3 1 2";
int G1, G2;
float x[MAX_GRADO], h[MAX_GRADO];
char out[1024]="";
char *token = strtok(cadena, " \t\r\n"); // obtenemos el primer token

    if (token != NULL) {
        G1 = atoi(token); // Convertimos el primer token a entero
        token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
    }

    if (token != NULL) {
        G2 = atoi(token); // Convertimos el segundo token a entero
        token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
    }

    for (int i = 0; i <= G1; i++) {
        if (token != NULL) {
            x[i] = atof(token); // Convertimos el token a float y lo almacenamos en x
            token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
        }
    }

    for (int i = 0; i <= G2; i++) {
        if (token != NULL) {
            h[i] = atof(token); // Convertimos el token a float y lo almacenamos en h
            token = strtok(NULL, " \t\r\n"); // Obtenemos el siguiente token
        }
    }

    for(int i=G1;i>=0;i--){
        if(i>0){
            sprintf(out+strlen(out), "%gx^%d+",x[i],i);
        } else {
            sprintf(out+strlen(out), "%g",x[i]);
        }
    }
    printf("sol: %s\n", out);

}