#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Enumeración para el resultado de búsqueda
typedef enum {
    NO,
    SI
} Resultado;

// Estructura para cada caso de prueba
typedef struct {
    char *titular;
    char *mensaje;
} Caso;

// Convierte una cadena a minúsculas (en el mismo buffer)
void convertirMinusculas(char *cadena) {
    for (int i = 0; cadena[i]; i++) {
        cadena[i] = tolower(cadena[i]);
    }
}

// Elimina los espacios del mensaje oculto
char *quitarEspacios(const char *mensaje) {
    int len = strlen(mensaje);
    char *resultado = (char *)malloc(len + 1);
    int j = 0;
    for (int i = 0; mensaje[i]; i++) {
        if (mensaje[i] != ' ')
            resultado[j++] = mensaje[i];
    }
    resultado[j] = '\0';
    return resultado;
}

// Verifica si el mensaje aparece en orden en el titular
Resultado contieneOculto(const char *titular, const char *mensaje) {
    int i = 0, j = 0;
    while (titular[i] && mensaje[j]) {
        if (titular[i] == mensaje[j]) {
            j++;
        }
        i++;
    }
    return mensaje[j] == '\0' ? SI : NO;
}

// Procesa cada caso y guarda el resultado en archivo y pantalla
void procesarCaso(Caso *caso, FILE *salida) {
    char *titular = strdup(caso->titular);
    char *mensaje = quitarEspacios(caso->mensaje);
    convertirMinusculas(titular);
    convertirMinusculas(mensaje);

    Resultado r = contieneOculto(titular, mensaje);
    if (r == SI) {
        printf("SI\n");
        fprintf(salida, "SI\n");
    } else {
        printf("NO\n");
        fprintf(salida, "NO\n");
    }

    free(titular);
    free(mensaje);
}

// Función recursiva para procesar todos los casos
void procesarCasosRecursivamente(FILE *entrada, FILE *salida, int casosRestantes) {
    if (casosRestantes == 0) return;

    char bufferTitular[2100], bufferMensaje[2100];
    if (!fgets(bufferTitular, sizeof(bufferTitular), entrada)) return;
    if (!fgets(bufferMensaje, sizeof(bufferMensaje), entrada)) return;

    // Elimina saltos de línea
    bufferTitular[strcspn(bufferTitular, "\n")] = '\0';
    bufferMensaje[strcspn(bufferMensaje, "\n")] = '\0';

    Caso caso;
    caso.titular = bufferTitular;
    caso.mensaje = bufferMensaje;

    procesarCaso(&caso, salida);

    procesarCasosRecursivamente(entrada, salida, casosRestantes - 1);
}

int main() {
    FILE *fentrada = fopen("entrada_milenio.txt", "r");
    FILE *fsalida = fopen("salida_milenio.txt", "w");

    if (!fentrada || !fsalida) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    int n;
    fscanf(fentrada, "%d\n", &n); // leer número de casos

    procesarCasosRecursivamente(fentrada, fsalida, n);

    fclose(fentrada);
    fclose(fsalida);
    return 0;
}
