#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iostream>

using namespace std;

#define MAX 7

typedef enum { SIGUE, TERMINADO } EstadoJuego;

typedef union {
    int valor;
    char texto[20];
} ResultadoUnion;

typedef struct {
    int matriz[MAX][MAX];
    int posF;
    int posC;
    int suma;
    EstadoJuego estado;
} Juego;

int contarChacalesDiagonal(int M[MAX][MAX], int i) {
    if (i == MAX) return 0;
    return (M[i][i] == 0 ? 1 : 0) + contarChacalesDiagonal(M, i + 1);
}

int laberinto(Juego *juego) {
    int chacales = 0;
    juego->suma = 0;

    for (int i = 0; i < MAX; i++) {
        if (juego->matriz[i][i] == 0) chacales++;
        juego->suma += juego->matriz[i][i];
        juego->posF = i + 1;
        juego->posC = i + 1;
        if (chacales == 3) {
            juego->estado = TERMINADO;
            return juego->suma;
        }
    }

    for (int i = MAX - 2; i >= 0; i--) {
        if (juego->matriz[i][MAX - 1] == 0) chacales++;
        juego->suma += juego->matriz[i][MAX - 1];
        juego->posF = i + 1;
        juego->posC = MAX;
        if (chacales == 3) {
            juego->estado = TERMINADO;
            return juego->suma;
        }
    }

    juego->estado = TERMINADO;
    return juego->suma;
}

void leerMatriz(const char *nombreArchivo, int M[MAX][MAX]) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        cout << "Error al abrir archivo de entrada.\n";
        exit(1);
    }

    int contador = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (fscanf(archivo, "%d", &M[i][j]) != 1) {
                cout << "Datos incorrectos.\n";
                fclose(archivo);
                exit(1);
            }
            contador++;
        }
    }

    if (contador != MAX * MAX) {
        cout << "Error: archivo incompleto.\n";
        exit(1);
    }

    fclose(archivo);
}

void guardarSalida(const char *nombreArchivo, Juego juego) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        cout << "Error al escribir archivo.\n";
        return;
    }

    fprintf(archivo, "Suma total: %d\n", juego.suma);
    fprintf(archivo, "Posición final: Fila %d, Columna %d\n", juego.posF, juego.posC);
    fprintf(archivo, "Estado: %s\n", juego.estado == TERMINADO ? "TERMINADO" : "SIGUE");
    fclose(archivo);
}

int main() {
    Juego *juego = (Juego *)malloc(sizeof(Juego));
    if (!juego) {
        cout << "Error de memoria.\n";
        return 1;
    }

    // Usar archivos fijos
    const char *archivoEntrada = "entrada_laberinto.txt";
    const char *archivoSalida = "salida_laberinto.txt";

    leerMatriz(archivoEntrada, juego->matriz);

    int chacalesDiag = contarChacalesDiagonal(juego->matriz, 0);
    cout << "Chacales en la diagonal: " << chacalesDiag << "\n";

    ResultadoUnion resultado;
    resultado.valor = laberinto(juego);

    cout << "Suma total: " << resultado.valor << "\n";
    cout << "Posicion final: Fila " << juego->posF << ", Columna " << juego->posC << "\n";
    cout << "Estado: " << (juego->estado == TERMINADO ? "TERMINADO" : "SIGUE") << "\n";

    guardarSalida(archivoSalida, *juego);
    free(juego);
    return 0;
}