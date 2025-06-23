#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum { VERTICAL, HORIZONTAL } Orientacion;

typedef struct {
    int fila;
    int columna;
    Orientacion orientacion;
    char *palabra;
} Jugada;

typedef struct {
    char **casillas;
    int filas;
    int columnas;
} Tablero;

const char *diccionario[] = {"CASA", "PERRO", "GATO", "JUEGO", "SCRABBLE", "HOLA", "MUNDO"};
const int diccionarioTam = 7;

// Prototipos
Tablero *crearTablero(int filas, int columnas);
void liberarTablero(Tablero *tablero);
void imprimirTablero(Tablero *tablero);
void guardarTablero(Tablero *tablero, const char *filename);
bool estaVacio(Tablero *tablero);
bool esPalabra(const char *palabra);
bool colocarPalabra(Tablero *tablero, Jugada jugada);
int contarOcupadasRecursivo(Tablero *tablero, int fila, int columna);
void limpiarBuffer();
bool palabraUsada(char **usadas, int usadasTam, const char *palabra);
void agregarPalabraUsada(char ***usadas, int *usadasTam, const char *palabra);

// Función principal
int main() {
    Tablero *tablero = crearTablero(9, 9);
    char palabra[20];
    int fila, columna, orient;
    char seguir;
    char archivo[] = "tablero.txt";
    char **palabrasUsadas = NULL;
    int usadasTam = 0;

    printf("=== SCRABBLE ANSI C ===\n");
    do {
        // Validar palabra no usada y que esté en el diccionario
        while (1) {
            printf("\nIngrese palabra (max 19 letras): ");
            scanf("%19s", palabra);
            limpiarBuffer();

            // Convertir a mayúsculas para comparar
            for (int i = 0; palabra[i]; i++) palabra[i] = toupper(palabra[i]);

            if (palabraUsada(palabrasUsadas, usadasTam, palabra)) {
                printf("La palabra ya fue usada en esta partida. Intente con otra.\n");
                continue;
            }
            if (!esPalabra(palabra)) {
                printf("La palabra no está en el diccionario. Intente con otra.\n");
                continue;
            }
            break;
        }

        printf("Ingrese fila inicial (0-8): ");
        while (scanf("%d", &fila) != 1 || fila < 0 || fila > 8) {
            printf("Fila invalida. Ingrese un valor entre 0 y 8: ");
            limpiarBuffer();
        }

        printf("Ingrese columna inicial (0-8): ");
        while (scanf("%d", &columna) != 1 || columna < 0 || columna > 8) {
            printf("Columna invalida. Ingrese un valor entre 0 y 8: ");
            limpiarBuffer();
        }

        printf("Ingrese orientacion (0=VERTICAL, 1=HORIZONTAL): ");
        while (scanf("%d", &orient) != 1 || (orient != 0 && orient != 1)) {
            printf("Orientacion invalida. Ingrese 0 o 1: ");
            limpiarBuffer();
        }
        limpiarBuffer();

        Jugada jugada;
        jugada.fila = fila;
        jugada.columna = columna;
        jugada.orientacion = (Orientacion)orient;
        jugada.palabra = palabra;

        if (colocarPalabra(tablero, jugada)) {
            printf("Palabra colocada con exito.\n");
            agregarPalabraUsada(&palabrasUsadas, &usadasTam, palabra);
        } else {
            printf("No se pudo colocar la palabra.\n");
        }

        imprimirTablero(tablero);
        guardarTablero(tablero, archivo);

        printf("Casillas ocupadas: %d\n", contarOcupadasRecursivo(tablero, 0, 0));

        // Validar respuesta s/n
        do {
            printf("Desea colocar otra palabra? (s/n): ");
            scanf(" %c", &seguir);
            limpiarBuffer();
            seguir = tolower(seguir);
            if (seguir != 's' && seguir != 'n') {
                printf("Respuesta invalida. Solo se acepta 's' o 'n'.\n");
            }
        } while (seguir != 's' && seguir != 'n');

    } while (seguir == 's');

    printf("Tablero final guardado en %s\n", archivo);

    // Liberar memoria de palabras usadas
    for (int i = 0; i < usadasTam; i++) free(palabrasUsadas[i]);
    free(palabrasUsadas);

    liberarTablero(tablero);
    return 0;
}

// Crea un tablero dinámicamente
Tablero *crearTablero(int filas, int columnas) {
    Tablero *tablero = (Tablero *)malloc(sizeof(Tablero));
    tablero->filas = filas;
    tablero->columnas = columnas;
    tablero->casillas = (char **)malloc(filas * sizeof(char *));
    for (int i = 0; i < filas; i++) {
        tablero->casillas[i] = (char *)calloc(columnas, sizeof(char));
    }
    return tablero;
}

// Libera la memoria del tablero
void liberarTablero(Tablero *tablero) {
    for (int i = 0; i < tablero->filas; i++) {
        free(tablero->casillas[i]);
    }
    free(tablero->casillas);
    free(tablero);
}

// Imprime el tablero en pantalla
void imprimirTablero(Tablero *tablero) {
    printf("\n   ");
    for (int j = 0; j < tablero->columnas; j++) printf("%d ", j);
    printf("\n");
    for (int i = 0; i < tablero->filas; i++) {
        printf("%d: ", i);
        for (int j = 0; j < tablero->columnas; j++) {
            char ch = tablero->casillas[i][j] ? tablero->casillas[i][j] : '.';
            printf("%c ", ch);
        }
        printf("\n");
    }
}

// Guarda el tablero en un archivo de texto
void guardarTablero(Tablero *tablero, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return;
    for (int i = 0; i < tablero->filas; i++) {
        for (int j = 0; j < tablero->columnas; j++) {
            char ch = tablero->casillas[i][j] ? tablero->casillas[i][j] : '.';
            fprintf(fp, "%c ", ch);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Verifica si el tablero está vacío
bool estaVacio(Tablero *tablero) {
    for (int i = 0; i < tablero->filas; i++)
        for (int j = 0; j < tablero->columnas; j++)
            if (tablero->casillas[i][j] != '\0') return false;
    return true;
}

// Verifica si la palabra está en el diccionario
bool esPalabra(const char *palabra) {
    for (int i = 0; i < diccionarioTam; i++)
        if (strcmp(palabra, diccionario[i]) == 0)
            return true;
    return false;
}

// Coloca la palabra en el tablero si es válida
bool colocarPalabra(Tablero *tablero, Jugada jugada) {
    int len = strlen(jugada.palabra);
    int i, fila = jugada.fila, col = jugada.columna;
    bool usaLetraTablero = false;

    // Verifica si la palabra cabe en el tablero
    if (jugada.orientacion == HORIZONTAL) {
        if (col + len > tablero->columnas) return false;
    } else {
        if (fila + len > tablero->filas) return false;
    }

    // Verifica si la palabra está en el diccionario
    if (!esPalabra(jugada.palabra)) return false;

    // Si el tablero está vacío, la primera letra debe ir en el centro
    if (estaVacio(tablero)) {
        int centro = tablero->filas / 2;
        if (!((jugada.orientacion == HORIZONTAL && fila == centro && col <= centro && col + len > centro) ||
              (jugada.orientacion == VERTICAL && col == centro && fila <= centro && fila + len > centro))) {
            return false;
        }
    }

    // Verifica que cada casilla esté vacía o coincida con la letra
    fila = jugada.fila; col = jugada.columna;
    for (i = 0; i < len; i++) {
        char actual = toupper(jugada.palabra[i]);
        if (tablero->casillas[fila][col] != '\0' && tablero->casillas[fila][col] != actual) return false;
        if (tablero->casillas[fila][col] == actual) usaLetraTablero = true;
        if (jugada.orientacion == HORIZONTAL) col++;
        else fila++;
    }

    // Si el tablero no está vacío, debe usar al menos una letra del tablero
    if (!estaVacio(tablero) && !usaLetraTablero) return false;

    // Coloca la palabra en el tablero
    fila = jugada.fila; col = jugada.columna;
    for (i = 0; i < len; i++) {
        tablero->casillas[fila][col] = toupper(jugada.palabra[i]);
        if (jugada.orientacion == HORIZONTAL) col++;
        else fila++;
    }
    return true;
}

// Función recursiva para contar casillas ocupadas
int contarOcupadasRecursivo(Tablero *tablero, int fila, int columna) {
    if (fila >= tablero->filas) return 0;
    if (columna >= tablero->columnas) return contarOcupadasRecursivo(tablero, fila + 1, 0);
    int ocupado = tablero->casillas[fila][columna] ? 1 : 0;
    return ocupado + contarOcupadasRecursivo(tablero, fila, columna + 1);
}

// Limpia el buffer de entrada
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Verifica si la palabra ya fue usada en la partida
bool palabraUsada(char **usadas, int usadasTam, const char *palabra) {
    for (int i = 0; i < usadasTam; i++)
        if (strcmp(usadas[i], palabra) == 0)
            return true;
    return false;
}

// Agrega la palabra usada a la lista dinámica
void agregarPalabraUsada(char ***usadas, int *usadasTam, const char *palabra) {
    *usadas = (char **)realloc(*usadas, (*usadasTam + 1) * sizeof(char *));
    (*usadas)[*usadasTam] = (char *)malloc(strlen(palabra) + 1);
    strcpy((*usadas)[*usadasTam], palabra);
    (*usadasTam)++;
}