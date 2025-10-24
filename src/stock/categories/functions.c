#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils/utils.h"

#define MAXCATEGORIA 30
#define MAXLINEAS 100
#define ARCHIVO "build/stock/categories/categories.txt"

typedef char tString[MAXCATEGORIA];
typedef tString tLista[MAXLINEAS];

int categoriaExiste(const char* nuevaCategoria) {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        return 0;
    }

    tString linea;
    while (fgets(linea, MAXCATEGORIA, archivo)) {
        linea[strcspn(linea, "\n")] = 0; // Quitar salto de línea
        if (strcmp(linea, nuevaCategoria) == 0) {
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);
    return 0;
}

void crearCategoria() {
    FILE* archivo = fopen(ARCHIVO, "a");
    if (!archivo) {
        printf("Error: No se pudo abrir o crear el archivo.\n");
        return;
    }

    tString nuevaCategoria;

    fflush(stdin);
    printf("Ingrese el nombre de la nueva categoria: ");
    scanf("%29[^\n]", nuevaCategoria);

    if (strlen(nuevaCategoria) == 0) {
        printf("Error: La categoria no puede estar vacia.\n");
        return;
    }

    mayus(nuevaCategoria);

    if (categoriaExiste(nuevaCategoria)) {
        printf("Error: La categoria '%s' ya existe.\n", nuevaCategoria);
        return;
    }

    fprintf(archivo, "%s\n", nuevaCategoria);
    fclose(archivo);

    printf("Categoria '%s' agregada exitosamente.\n", nuevaCategoria);
}

void leerCategorias() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo. Puede que no exista aun.\n");
        return;
    }

    tLista categorias;
    int count = 0;

    while (fgets(categorias[count], MAXCATEGORIA, archivo) && count < MAXLINEAS) {
        categorias[count][strcspn(categorias[count], "\n")] = 0; // quitar salto de línea
        count++;
    }

    fclose(archivo);

    if (count == 0) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    printf("Categorias actuales:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, categorias[i]);
    }
}

void filtrarCategoriaPorNombre() {
    tString categoria;

    fflush(stdin);
    printf("Ingrese el nombre de la categoria: ");
    scanf("%29[^\n]", categoria);

    if (strlen(categoria) == 0) {
        printf("Error: La categoria no puede estar vacia.\n");
        return;
    }

    mayus(categoria);

    if (!categoriaExiste(categoria)) {
        printf("La categoria '%s' no existe.\n", categoria);
        return;
    }

    printf("La categoria '%s' existe.\n", categoria);
}

void buscarCategorias(void) {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tString busqueda;
    printf("Ingrese la categoria a buscar: ");
    fflush(stdin);
    scanf(" %29[^\n]", busqueda);
    
    if (strlen(busqueda) == 0) {
        printf("Error: La busqueda no puede estar vacia.\n");
        fclose(archivo);
        return;
    }

    mayus(busqueda);

    tLista categorias;
    int coincidencias[MAXLINEAS];
    int count = 0;
    int totalCoincidencias = 0;

    while (count < MAXLINEAS && fgets(categorias[count], MAXCATEGORIA, archivo)) {
        categorias[count][strcspn(categorias[count], "\n")] = '\0';

        tString temp;
        strcpy(temp, categorias[count]);
        mayus(temp);

        if (strstr(temp, busqueda) != NULL) {
            coincidencias[totalCoincidencias++] = count;
        }

        count++;
    }

    fclose(archivo);

    if (totalCoincidencias == 0) {
        printf("No se encontraron categorias que contengan '%s'.\n", busqueda);
    } else {
        if (totalCoincidencias == 1)
            printf("\nSe encontró una categoria que contiene '%s':\n", busqueda);
        else
            printf("\nSe encontraron %d categorias que contienen '%s':\n", totalCoincidencias, busqueda);

        for (int i = 0; i < totalCoincidencias; i++) {
            printf("%d. %s\n", i + 1, categorias[coincidencias[i]]);
        }
    }
}

void eliminarCategoria() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tLista categorias;
    int count = 0;

    while (count < MAXLINEAS && fgets(categorias[count], MAXCATEGORIA, archivo)) {
        categorias[count][strcspn(categorias[count], "\n")] = '\0';
        count++;
    }
    fclose(archivo);

    if (count == 0) {
        printf("No hay categorias para eliminar.\n");
        return;
    }

    // Buscar categoría
    tString categoria;
    printf("Ingrese el nombre de la categoria a eliminar: ");
    fflush(stdin);
    scanf(" %29[^\n]", categoria);
    
    if (strlen(categoria) == 0) {
        printf("Error: La categoria no puede estar vacia.\n");
        return;
    }

    mayus(categoria);

    // Buscar coincidencia exacta
    int indiceEncontrado = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(categorias[i], categoria) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: La categoria '%s' no se encontro.\n", categoria);
        return;
    }

    // Reescribir archivo sin la categoría eliminada
    archivo = fopen(ARCHIVO, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i != indiceEncontrado) {
            fprintf(archivo, "%s\n", categorias[i]);
        }
    }
    fclose(archivo);

    printf("Categoria '%s' eliminada exitosamente.\n", categoria);
}