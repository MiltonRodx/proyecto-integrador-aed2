#include "../../utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCATEGORIA 30
#define MAXLINEAS 100
#define ARCHIVO "build/stock/categories/categories.txt"

typedef char tString[MAXCATEGORIA];

typedef struct {
    tString* datos;
    int tam;
} tLista;

int escribirArchivo(const char* nombreArchivo, const char* linea) {
    FILE* archivo = fopen(nombreArchivo, "a");
    if (!archivo) {
        return 0;
    }

    fprintf(archivo, "%s\n", linea);
    fclose(archivo);

    return 1;
}

tLista leerArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        return (tLista){NULL, 0};
    }

    tLista lista = {NULL, 0};
    char linea[MAXCATEGORIA];

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        lista.datos = realloc(lista.datos, (lista.tam + 1) * sizeof(tString));
        if (!lista.datos) {
            printf("Error al reasignar memoria");
            fclose(archivo);
            return (tLista){NULL, 0};
        }

        strcpy(lista.datos[lista.tam], linea);
        lista.tam++;
    }

    fclose(archivo);
    return lista;
}

int categoriaExiste(const char* nuevaCategoria) {
    tLista categorias = leerArchivo(ARCHIVO);

    for (int i = 0; i < categorias.tam; i++) {
        if (strcmp(categorias.datos[i], nuevaCategoria) == 0) {
            return 1;
        }
    }

    free(categorias.datos);
    return 0;
}

void ingresarCategoria(tString categoria) {
    int salir = 0;

    do {
        fflush(stdin);
        printf("Ingrese el nombre de la categoria: ");
        scanf(" %29[^\n]", categoria);

        if (strlen(categoria) == 0) {
            printf("Error: La categoria no puede estar vacia.\n\n");
            salir = 0;
        } else {
            mayus(categoria);
            salir = 1;
        }

    } while (salir == 0);
}

void crearCategoria() {
    tString nuevaCategoria;

    ingresarCategoria(nuevaCategoria);

    if (categoriaExiste(nuevaCategoria)) {
        printf("Error: La categoria '%s' ya existe.\n", nuevaCategoria);
        return;
    }

    if (escribirArchivo(ARCHIVO, nuevaCategoria)) {
        printf("Categoria '%s' agregada exitosamente.\n", nuevaCategoria);
    } else {
        printf("Error: No se pudo agregar la categoria.\n");
    }
}

void crearCategorias() {
    int salir = 0;
    do {
        crearCategoria();
        printf("Desea agregar otra categoria? 1-Si 2-No: ");
        scanf("%d", &salir);
    } while (salir == 1);
}

void leerCategorias() {
    tLista categorias = leerArchivo(ARCHIVO);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    printf("Categorias actuales:\n");

    for (int i = 0; i < categorias.tam; i++) {
        printf("%d. %s\n", i + 1, categorias.datos[i]);
    }

    free(categorias.datos);
}

void buscarCategorias(void) {
    tLista categorias = leerArchivo(ARCHIVO);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    tString busqueda;

    ingresarCategoria(busqueda);

    int coincidencias[MAXLINEAS];
    int totalCoincidencias = 0;

    // Buscar coincidencias en las categorías
    for (int i = 0; i < categorias.tam; i++) {
        if (strstr(categorias.datos[i], busqueda) != NULL) {
            coincidencias[totalCoincidencias++] = i;
        }
    }

    if (totalCoincidencias == 0) {
        printf("No se encontraron categorias que contengan '%s'.\n", busqueda);
    } else {
        if (totalCoincidencias == 1)
            printf("\nSe encontro 1 categoria que contiene '%s':\n", busqueda);
        else
            printf("\nSe encontraron %d categorias que contienen '%s':\n", totalCoincidencias, busqueda);

        for (int i = 0; i < totalCoincidencias; i++) {
            printf("%d. %s\n", i + 1, categorias.datos[coincidencias[i]]);
        }
    }

    free(categorias.datos);
}

void filtrarCategoriaPorNombre() {
    tString categoria;

    ingresarCategoria(categoria);

    if (!categoriaExiste(categoria)) {
        printf("La categoria '%s' no existe.\n", categoria);
        return;
    }

    printf("La categoria '%s' existe.\n", categoria);
}

void eliminarCategoria() {
    tString categoria;

    ingresarCategoria(categoria);

    if (!categoriaExiste(categoria)) {
        printf("La categoria '%s' no existe.\n", categoria);
        return;
    }

    tLista categorias = leerArchivo(ARCHIVO);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    FILE* archivo = fopen(ARCHIVO, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        free(categorias.datos);
        return;
    }

    for (int i = 0; i < categorias.tam; i++) {
        if (strcmp(categorias.datos[i], categoria) != 0) {
            fprintf(archivo, "%s\n", categorias.datos[i]);
        }
    }

    fclose(archivo);
    free(categorias.datos);

    printf("Categoria '%s' eliminada exitosamente.\n", categoria);
}