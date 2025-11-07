#include "../../utils/utils.h"
#include "categories.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int escribirArchivo(const char* nombreArchivo, const char* linea) {
    FILE* archivo = fopen(nombreArchivo, "a");
    if (!archivo) {
        return 0;
    }

    fprintf(archivo, "%s\n", linea);
    fclose(archivo);

    return 1;
}

static tListaCategorias leerArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        return (tListaCategorias){NULL, 0};
    }

    tListaCategorias lista = {NULL, 0};
    char linea[MAXCATEGORIA];

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        lista.datos = realloc(lista.datos, (lista.tam + 1) * sizeof(tString));
        if (!lista.datos) {
            printf("Error al reasignar memoria");
            fclose(archivo);
            return (tListaCategorias){NULL, 0};
        }

        strcpy(lista.datos[lista.tam], linea);
        lista.tam++;
    }

    fclose(archivo);
    return lista;
}

int categoriaExiste(const char* nuevaCategoria) {
    tListaCategorias categorias = leerArchivo(ARCHIVO_CATEGORIAS);

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

    if (escribirArchivo(ARCHIVO_CATEGORIAS, nuevaCategoria)) {
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

static void mostrarCategoriasEnColumnas(tListaCategorias categorias) {
    int columnas = 4;
    int filas = (categorias.tam + columnas - 1) / columnas;

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            int indice = i + j * filas;
            if (indice < categorias.tam) {
                printf("%02d. %-25s", indice + 1, categorias.datos[indice]);
            }
        }
        printf("\n");
    }
}

void leerCategorias() {
    tListaCategorias categorias = leerArchivo(ARCHIVO_CATEGORIAS);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    printf("Categorias actuales:\n\n");
    mostrarCategoriasEnColumnas(categorias);

    free(categorias.datos);
}

void buscarCategorias(void) {
    tListaCategorias categorias = leerArchivo(ARCHIVO_CATEGORIAS);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    tString busqueda;
    ingresarCategoria(busqueda);

    int coincidencias[MAXLINEAS];
    int totalCoincidencias = 0;

    for (int i = 0; i < categorias.tam; i++) {
        if (strstr(categorias.datos[i], busqueda) != NULL) {
            coincidencias[totalCoincidencias++] = i;
        }
    }

    if (totalCoincidencias == 0) {
        printf("No se encontraron categorias que contengan '%s'.\n", busqueda);
    } else {
        if (totalCoincidencias == 1)
            printf("\nSe encontro 1 categoria que contiene '%s':\n\n", busqueda);
        else
            printf("\nSe encontraron %d categorias que contienen '%s':\n\n", totalCoincidencias, busqueda);

        tListaCategorias resultados;
        resultados.datos = (tString*)malloc(totalCoincidencias * sizeof(tString));
        resultados.tam = totalCoincidencias;

        for (int i = 0; i < totalCoincidencias; i++) {
            strcpy(resultados.datos[i], categorias.datos[coincidencias[i]]);
        }

        mostrarCategoriasEnColumnas(resultados);

        free(resultados.datos);
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

void editarCategoria() {
    tListaCategorias categorias = leerArchivo(ARCHIVO_CATEGORIAS);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    tString categoriaVieja;
    printf("[Antigua] ");
    ingresarCategoria(categoriaVieja);

    if (!categoriaExiste(categoriaVieja)) {
        printf("La categoria '%s' no existe.\n", categoriaVieja);
        return;
    }

    tString categoriaNueva;

    printf("[Nueva] ");
    ingresarCategoria(categoriaNueva);

    if (categoriaExiste(categoriaNueva)) {
        printf("Error: La categoria '%s' ya existe.\n", categoriaNueva);
        return;
    }

    FILE* archivo = fopen(ARCHIVO_CATEGORIAS, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        free(categorias.datos);
        return;
    }

    for (int i = 0; i < categorias.tam; i++) {
        if (strcmp(categorias.datos[i], categoriaVieja) == 0) {
            fprintf(archivo, "%s\n", categoriaNueva);
        } else {
            fprintf(archivo, "%s\n", categorias.datos[i]);
        }
    }

    fclose(archivo);
    free(categorias.datos);

    printf("Categoria '%s' editada a '%s' exitosamente.\n", categoriaVieja, categoriaNueva);
}

void eliminarCategoria() {
    tString categoria;

    ingresarCategoria(categoria);

    if (!categoriaExiste(categoria)) {
        printf("La categoria '%s' no existe.\n", categoria);
        return;
    }

    tListaCategorias categorias = leerArchivo(ARCHIVO_CATEGORIAS);

    if (!categorias.datos) {
        printf("No hay categorias en el archivo.\n");
        return;
    }

    FILE* archivo = fopen(ARCHIVO_CATEGORIAS, "w");
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