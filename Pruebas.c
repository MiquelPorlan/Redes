#include <cstddef>
int main() {
    char cadena[] = "Hola,que,tal,mundo";
    char *token = strtok(cadena, ","); // Usamos la coma como delimitador

    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, ","); // Siguientes llamadas con NULL
    }

    return 0;
}