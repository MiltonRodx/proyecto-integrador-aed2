#include <stdio.h>
#include "stock.h"
#include "categories/categories.h"

void menuStock() {
    int opcion;
    do {
        printf("\n--- MENU DE STOCK ---\n");
        printf("1. Categorias\n");
        printf("2. Volver al menu principal\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                menuCategories();
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while (opcion != 2);
}