#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define TAM 9

typedef enum { VERTICAL, HORIZONTAL } orientacion;

// Retorna true si el tablero está vacío
bool estaVacio(char T[TAM][TAM]) {
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            if (T[i][j] != ' ') return false;
    return true;
}

// Simula un diccionario simple
bool esPalabra(char *palabra) {
    char *diccionario[] = { "AVION", "VINO", "ION", "OIN", "ARBOL" };
    int n = sizeof(diccionario) / sizeof(diccionario[0]);
    for (int i = 0; i < n; i++)
        if (strcmp(diccionario[i], palabra) == 0)
            return true;
    return false;
}

// Imprime el tablero en consola y en archivo
void imprimirTablero(char T[TAM][TAM], FILE *salida) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            printf("%c ", T[i][j]);
            fprintf(salida, "%c ", T[i][j]);
        }
        printf("\n");
        fprintf(salida, "\n");
    }
    printf("\n");
    fprintf(salida, "\n");
}

// Intenta colocar una palabra en el tablero
bool colocarPalabra(char T[TAM][TAM], int f, int c, orientacion o, char *palabra) {
    int len = strlen(palabra);
    if ((o == HORIZONTAL && c + len > TAM) || (o == VERTICAL && f + len > TAM)) return false;
    if (!esPalabra(palabra)) return false;

    bool usaLetra = false;

    if (estaVacio(T)) {
        int centro = TAM / 2;
        bool pasaPorCentro = false;
        for (int i = 0; i < len; i++) {
            int fila = f + (o == VERTICAL ? i : 0);
            int col  = c + (o == HORIZONTAL ? i : 0);
            if (fila == centro && col == centro) {
                pasaPorCentro = true;
                break;
            }
        }
        if (!pasaPorCentro) return false;
    } else {
        for (int i = 0; i < len; i++) {
            int fila = f + (o == VERTICAL ? i : 0);
            int col  = c + (o == HORIZONTAL ? i : 0);
            if (T[fila][col] != ' ' && T[fila][col] != toupper(palabra[i])) return false;
            if (T[fila][col] == toupper(palabra[i])) usaLetra = true;
        }
        if (!usaLetra) return false;
    }

    for (int i = 0; i < len; i++) {
        int fila = f + (o == VERTICAL ? i : 0);
        int col  = c + (o == HORIZONTAL ? i : 0);
        T[fila][col] = toupper(palabra[i]);
    }

    return true;
}

int main() {
    FILE *fsalida = fopen("salida_scrabble.txt", "w");
    if (!fsalida) {
        printf("No se pudo abrir el archivo de salida.\n");
        return 1;
    }

    char tablero[TAM][TAM];
    for (int i = 0; i < TAM; i++)
        for (int j = 0; j < TAM; j++)
            tablero[i][j] = ' ';

    // Palabras que sí se pueden colocar por coincidencia exacta
    char *palabras[] = { "AVION", "VINO", "ION", "OIN", "ARBOL" };
    int filas[]      = {    0,     1,      2,     3,     3 };
    int columnas[]   = {    4,     4,      5,     4,     2 };
    orientacion orientaciones[] = { VERTICAL, HORIZONTAL, VERTICAL, HORIZONTAL, HORIZONTAL };


    int total = sizeof(palabras) / sizeof(palabras[0]);

    for (int i = 0; i < total; i++) {
        bool exito = colocarPalabra(tablero, filas[i], columnas[i], orientaciones[i], palabras[i]);
        fprintf(fsalida, "Colocando \"%s\" en (%d,%d) %s: %s\n",
                palabras[i], filas[i], columnas[i],
                orientaciones[i] == VERTICAL ? "VERTICAL" : "HORIZONTAL",
                exito ? "EXITO" : "FALLO");

        printf("Colocando \"%s\" en (%d,%d) %s: %s\n",
                palabras[i], filas[i], columnas[i],
                orientaciones[i] == VERTICAL ? "VERTICAL" : "HORIZONTAL",
                exito ? "EXITO" : "FALLO");

        imprimirTablero(tablero, fsalida);
    }

    fclose(fsalida);
    return 0;
}
