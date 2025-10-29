#include "products.h"
#include "functions.h"
#include <stdio.h>

void crearMenuProductos() {
    printf("\n---| MENU DE PRODUCTOS |---\n");
    printf("[1] Agregar\n");
    printf("[2] Listar\n");
    printf("[3] Buscar\n");
    printf("[4] Actualizar\n");
    printf("[5] Eliminar\n");
    printf("[x] Volver al menu principal\n");
    printf("Opcion: ");
}

void menuProductos() {
    char opcion;
    int resultado;

    do {
        crearMenuProductos();

        resultado = scanf(" %c", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            crearProductos();
            break;
        case '2':
            obtenerProductos();
            break;
        case '3':
            menuBusqueda();
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