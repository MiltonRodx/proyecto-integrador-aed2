#include "stock.h"
#include "categories/categories.h"
#include <stdio.h>

void crearMenuStock() {
    printf("\n---| MENU DE STOCK |---\n");
    printf("[1] Categorias\n");
    printf("[3] Productos\n");
    printf("[2] Volver al menu principal\n");
    printf("Opcion: ");
}

void menuStock() {
    int opcion;
    int resultado;

    do {
        crearMenuStock();
        resultado = scanf("%d", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case 1:
            menuCategorias();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 3);
}