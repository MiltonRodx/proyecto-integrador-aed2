#include "categories.h"
#include "functions.h"
#include <stdio.h>

void crearMenuCategorias() {
    printf("\n---| MENU DE CATEGORIAS |---\n");
    printf("[1] Agregar\n");
    printf("[2] Listar\n");
    printf("[3] Buscar\n");
    printf("[4] Filtrar\n");
    printf("[5] Eliminar\n");
    printf("[6] Volver al menu principal\n");
    printf("Opcion: ");
}

void menuCategorias() {
    int opcion;
    int resultado;

    do {
        crearMenuCategorias();

        resultado = scanf("%d", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case 1:
            crearCategoria();
            break;
        case 2:
            leerCategorias();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}