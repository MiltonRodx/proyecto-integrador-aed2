#include "stock.h"
#include "categories/categories.h"
#include "products/products.h"
#include <stdio.h>
#include <ctype.h>

void crearMenuStock() {
    printf("\n---| MENU DE STOCK |---\n");
    printf("[1] Categorias\n");
    printf("[2] Productos\n");
    printf("[x] Volver al menu principal\n");
    printf("Seleccione una opcion: ");
}

void menuStock() {
    char opcion;

    do {
        crearMenuStock();
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            menuCategorias();
            break;
        case '2':
            menuProductos();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}