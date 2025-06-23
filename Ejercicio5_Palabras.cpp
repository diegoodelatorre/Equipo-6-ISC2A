#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
using namespace std;

#define MAX 50

// Enumeración para tipos de palabras
enum TipoPalabra { SUSTANTIVO = 1, VERBO = 2, ADJETIVO = 3 };

// Estructura del juego
typedef struct {
    int tipo;                // 1 = sustantivo, 2 = verbo, 3 = adjetivo
    char palabra[20];        // Palabra completa
    char pista[100];         // Pista
    char ini[5];             // Parte inicial
    char fin[5];             // Parte final
} juegoT;

// Procedimiento para cargar palabras desde archivo
void cargarJuego(ifstream &in, juegoT A[], int *tam) {
    *tam = 0;
    char linea[150];
    while (in.getline(linea, 150)) {
        char *token = strtok(linea, ",");
        if (!token) break;
        A[*tam].tipo = atoi(token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].palabra, token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].pista, token);

        token = strtok(NULL, ",");
        strcpy(A[*tam].ini, token);

        token = strtok(NULL, ",");
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
string tipoPalabraTexto(int tipo) {
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

    ifstream entrada("info.dat");
    if (!entrada) {
        cout << "No se pudo abrir el archivo info.dat\n";
        return 1;
    }

    cargarJuego(entrada, palabras, &tam);
    entrada.close();

    ofstream salida("salida.dat");
    if (!salida) {
        cout << "No se pudo abrir archivo salida.dat\n";
        return 1;
    }

    int totalPuntaje = 0;
    for (int ronda = 1; ronda <= 10; ronda++) {
        int indice = rand() % tam;
        juegoT actual = palabras[indice];

        cout << "Adivina un: " << tipoPalabraTexto(actual.tipo) << endl;
        cout << "Pista: " << actual.pista << endl;
        cout << "Ingresa una palabra: ";

        char respuesta[30];
        cin >> respuesta;

        int puntos = puntajePalabra(respuesta, actual);
        totalPuntaje += puntos;

        cout << "Puntos: " << puntos << "\n\n";
        salida << respuesta << "," << puntos << endl;
    }

    cout << "Puntaje total: " << totalPuntaje << endl;
    salida.close();
    return 0;
}
