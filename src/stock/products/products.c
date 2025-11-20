#include "products.h"
#include <ctype.h>
#include <stdio.h>

void crearMenuProductos() {
    printf("\n---| MENU DE PRODUCTOS |---\n");
    printf("[1] Agregar\n");
    printf("[2] Listar\n");
    printf("[3] Buscar\n");
    printf("[4] Actualizar\n");
    printf("[5] Eliminar\n");
    printf("[x] Volver\n");
    printf("Seleccione una opcion: ");
}

void menuProductos() {
    char opcion;

    do {
        crearMenuProductos();

        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            crearProductos();
            break;
        case '2':
            obtenerProductos();
            break;
        case '3':
            menuBusquedaProductos();
            break;
        case '4':
            actualizarProducto();
            break;
        case '5':
            eliminarProducto();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}