#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 50

// Enumeración para tipos de palabras
enum TipoPalabra { SUSTANTIVO = 1, VERBO = 2, ADJETIVO = 3 };

// Estructura del juego
typedef struct {
    int tipo;
    char palabra[20];
    char pista[100];
    char ini[5];
    char fin[5];
} juegoT;

// Procedimiento para cargar palabras desde archivo
void cargarJuego(FILE *in, juegoT A[], int *tam) {
    *tam = 0;
    char linea[150];
    while (fgets(linea, sizeof(linea), in)) {
        char *token = strtok(linea, ",");
        if (!token) break;
        A[*tam].tipo = atoi(token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].palabra, token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].pista, token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].ini, token);

        token = strtok(NULL, ",\n");
        strcpy(A[*tam].fin, token);

        (*tam)++;
    }
}

// Función que calcula puntaje según coincidencias
int puntajePalabra(char *cad, juegoT e) {
    if (strcmp(cad, e.palabra) == 0) return 10;

    int puntos = 0;
    if (strncmp(cad, e.ini, strlen(e.ini)) == 0) puntos += 3;

    int lenCad = strlen(cad);
    int lenFin = strlen(e.fin);
    if (lenCad >= lenFin && strcmp(cad + lenCad - lenFin, e.fin) == 0) puntos += 2;

    return puntos;
}

// Función que convierte tipo numérico a texto
const char* tipoPalabraTexto(int tipo) {
    switch (tipo) {
        case 1: return "sustantivo";
        case 2: return "verbo";
        case 3: return "adjetivo";
        default: return "desconocido";
    }
}

int main() {
    srand(time(NULL));
    juegoT palabras[MAX];
    int tam;

    FILE *entrada = fopen("info.dat", "r");
    if (!entrada) {
        printf("No se pudo abrir el archivo info.dat\n");
        return 1;
    }

    cargarJuego(entrada, palabras, &tam);
    fclose(entrada);

    FILE *salida = fopen("salida.dat", "w");
    if (!salida) {
        printf("No se pudo abrir archivo salida.dat\n");
        return 1;
    }

    int totalPuntaje = 0;
    for (int ronda = 1; ronda <= 10; ronda++) {
        int indice = rand() % tam;
        juegoT actual = palabras[indice];

        printf("Adivina un: %s\n", tipoPalabraTexto(actual.tipo));
        printf("Pista: %s\n", actual.pista);
        printf("Ingresa una palabra: ");

        char respuesta[30];
        scanf("%s", respuesta);

        int puntos = puntajePalabra(respuesta, actual);
        totalPuntaje += puntos;

        printf("Puntos: %d\n\n", puntos);
        fprintf(salida, "%s,%d\n", respuesta, puntos);
    }

    printf("Puntaje total: %d\n", totalPuntaje);
    fclose(salida);
    return 0;
}
