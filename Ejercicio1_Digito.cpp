#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <iostream> 

using namespace std;

// Enumeración para representar el estado de validación
typedef enum {INVALIDO, VALIDO} Estado;

// Unión para mostrar resultado como texto o número
typedef union {
    Estado estado;
    char texto[10];
} Resultado;

// Estructura para representar un ISBN
typedef struct {
    char codigo[14]; // 13 dígitos + '\0'
    int digitos[13];
} ISBN;

// Función para llenar arreglo con los dígitos del ISBN
void llenarISBN(char *codigo, int A[13]) {
    for (int i = 0; i < 13; i++)
        A[i] = codigo[i] - '0';
}

// Función recursiva para validar si todos los caracteres son dígitos
bool esNumeroRec(const char *cadena) {
    if (*cadena == '\0') return true;
    return (*cadena >= '0' && *cadena <= '9') && esNumeroRec(cadena + 1);
}

// Función para validar el ISBN
bool ISBNValido(int A[13]) {
    int suma = 0;
    for (int i = 0; i < 12; i++)
        suma += A[i] * (i % 2 == 0 ? 1 : 3);
    
    int residuo = suma % 10;
    int resultado = (10 - residuo) % 10;

    return resultado == A[12];
}

// Función para leer el archivo de entrada, procesar y escribir salida
void procesarArchivo(const char *entrada, const char *salida) {
    FILE *fin = fopen(entrada, "r");
    FILE *fout = fopen(salida, "w");

    if (!fin || !fout) {
        cout << "Error al abrir archivos." << endl;
        return;
    }

    char linea[20];
    while (fgets(linea, sizeof(linea), fin)) {
        ISBN *libro = (ISBN *)malloc(sizeof(ISBN));
        strncpy(libro->codigo, linea, 13);
        libro->codigo[13] = '\0';

        Resultado r;

        if (strlen(libro->codigo) != 13 || !esNumeroRec(libro->codigo)) {
            r.estado = INVALIDO;
            strcpy(r.texto, "INVALIDO");
        } else {
            llenarISBN(libro->codigo, libro->digitos);
            r.estado = ISBNValido(libro->digitos) ? VALIDO : INVALIDO;
            strcpy(r.texto, r.estado == VALIDO ? "VALIDO" : "INVALIDO");
        }

        fprintf(fout, "ISBN: %s - %s\n", libro->codigo, r.texto);
        cout << "ISBN: " << libro->codigo << " - " << r.texto << endl;

        free(libro);
    }

    fclose(fin);
    fclose(fout);
}
  
int main() {
    char entrada[50], salida[50];
    cout << "Nombre del archivo de entrada (ej. entrada.txt): ";
    cin >> entrada;
    cout << "Nombre del archivo de salida (ej. salida.txt): ";
    cin >> salida;

    procesarArchivo(entrada, salida);
    return 0;
}