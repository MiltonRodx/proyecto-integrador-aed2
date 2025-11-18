#include "orders.h"
#include <stdio.h>
#include <ctype.h>

void crearMenuPedidos() {
    printf("\n---| MENU DE PEDIDOS |---\n");
    printf("[1] Comenzar compra\n");
    printf("[x] Volver al menu principal\n");
    printf("Seleccione una opcion: ");
}

void menuPedidos() {
    char opcion;

    do {
        crearMenuPedidos();
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            menuCarrito();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}