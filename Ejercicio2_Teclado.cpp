#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enumeración para comandos especiales
typedef enum {
    COMANDO_INICIO,
    COMANDO_FIN,
    COMANDO_DERECHA,
    COMANDO_SUPR,
    COMANDO_INSERTAR
} Comando;

// Estructura del editor
typedef struct {
    char *texto;   // Texto dinámico
    int tam;       // Tamaño actual
    int cursor;    // Posición del cursor
} Editor;

// Crea un editor vacío
Editor *crearEditor() {
    Editor *e = (Editor *)malloc(sizeof(Editor));
    e->texto = (char *)malloc(sizeof(char));
    e->texto[0] = '\0';
    e->tam = 0;
    e->cursor = 0;
    return e;
}

// Libera memoria del editor
void liberarEditor(Editor *e) {
    free(e->texto);
    free(e);
}

// Inserta un carácter en la posición del cursor
void insertarCaracter(Editor *e, char c) {
    e->texto = (char *)realloc(e->texto, e->tam + 2); // espacio extra para nuevo char y '\0'
    for (int i = e->tam; i >= e->cursor; i--)
        e->texto[i + 1] = e->texto[i];
    e->texto[e->cursor] = c;
    e->cursor++;
    e->tam++;
}

// Borra el carácter a la derecha del cursor (como tecla Supr)
void borrarCaracter(Editor *e) {
    if (e->cursor >= e->tam) return; // nada que borrar
    for (int i = e->cursor; i < e->tam; i++)
        e->texto[i] = e->texto[i + 1];
    e->tam--;
    e->texto = (char *)realloc(e->texto, e->tam + 1);
}

// Mueve el cursor según el comando
void moverCursor(Editor *e, Comando comando) {
    if (comando == COMANDO_INICIO)
        e->cursor = 0;
    else if (comando == COMANDO_FIN)
        e->cursor = e->tam;
    else if (comando == COMANDO_DERECHA && e->cursor < e->tam)
        e->cursor++;
    else if (comando == COMANDO_SUPR)
        borrarCaracter(e);
}

// Procesa una línea de entrada y construye el texto final
void procesarLinea(Editor *e, const char *linea) {
    for (int i = 0; linea[i] != '\0' && linea[i] != '\n'; i++) {
        char c = linea[i];
        switch (c) {
            case '-':
                moverCursor(e, COMANDO_INICIO);
                break;
            case '+':
                moverCursor(e, COMANDO_FIN);
                break;
            case '*':
                moverCursor(e, COMANDO_DERECHA);
                break;
            case '3':
                moverCursor(e, COMANDO_SUPR);
                break;
            default:
                insertarCaracter(e, c);
        }
    }
}

// Escribe la línea final a salida en pantalla y archivo
void escribirSalida(FILE *archivo, const char *texto) {
    printf("%s\n", texto);
    fprintf(archivo, "%s\n", texto);
}

// Procesa las líneas recursivamente (extra para incluir recursividad)
void procesarLineasRecursivamente(FILE *entrada, FILE *salida) {
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), entrada) == NULL)
        return;

    Editor *e = crearEditor();
    procesarLinea(e, buffer);
    escribirSalida(salida, e->texto);
    liberarEditor(e);

    procesarLineasRecursivamente(entrada, salida);
}

int main() {
    FILE *fentrada = fopen("entrada_teclado.txt", "r");
    FILE *fsalida = fopen("salida_teclado.txt", "w");

    if (!fentrada || !fsalida) {
        printf("Error abriendo archivos.\n");
        return 1;
    }

    procesarLineasRecursivamente(fentrada, fsalida);

    fclose(fentrada);
    fclose(fsalida);
    return 0;
}