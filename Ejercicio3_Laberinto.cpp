#include <iostream>
#include <fstream>
#include <cstdlib>

#define MAX 7
using namespace std;

// Enumeración para estado del juego
enum EstadoJuego { SIGUE, TERMINADO };

// Unión para mostrar resultado como valor o texto
union ResultadoUnion {
    int valor;
    char texto[20];
};

// Estructura para almacenar el estado del juego
struct Juego {
    int matriz[MAX][MAX];
    int posF;
    int posC;
    int suma;
    EstadoJuego estado;
};

// Función recursiva para contar chacales en la diagonal principal
int contarChacalesDiagonal(int M[MAX][MAX], int i) {
    if (i == MAX) return 0;
    return (M[i][i] == 0 ? 1 : 0) + contarChacalesDiagonal(M, i + 1);
}

// Función que recorre el laberinto y realiza la suma
int laberinto(Juego *juego) {
    int chacales = 0;
    juego->suma = 0;

    // Recorre la diagonal principal
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

    // Recorre la última columna hacia arriba (sin repetir la esquina inferior derecha)
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

// Función para leer la matriz desde archivo
void leerMatriz(const char *nombreArchivo, int M[MAX][MAX]) {
    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "Error al abrir archivo de entrada." << endl;
        exit(1);
    }

    int valor;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (!(archivo >> valor)) {
                cout << "Datos insuficientes o incorrectos en el archivo." << endl;
                exit(1);
            }
            M[i][j] = valor;
        }
    }

    archivo.close();
}

// Función para guardar la salida en un archivo
void guardarSalida(const char *nombreArchivo, Juego juego) {
    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "Error al escribir archivo de salida." << endl;
        return;
    }

    archivo << "Suma total: " << juego.suma << endl;
    archivo << "Posición final -> Fila: " << juego.posF << ", Columna: " << juego.posC << endl;
    archivo << "Estado: " << (juego.estado == TERMINADO ? "TERMINADO" : "SIGUE") << endl;
    archivo.close();
}

int main() {
    Juego *juego = new Juego;
    leerMatriz("entrada_laberinto.txt", juego->matriz);

    // Contar chacales de forma recursiva
    int chacalesDiag = contarChacalesDiagonal(juego->matriz, 0);
    cout << "Chacales en la diagonal: " << chacalesDiag << endl;

    ResultadoUnion resultado;
    resultado.valor = laberinto(juego);

    cout << "Suma total: " << resultado.valor << endl;
    cout << "Posición final -> Fila: " << juego->posF << ", Columna: " << juego->posC << endl;
    cout << "Estado: " << (juego->estado == TERMINADO ? "TERMINADO" : "SIGUE") << endl;

    guardarSalida("salida.txt", *juego);

    delete juego;
    return 0;
}
