#include "carts.h"
#include "../../users/users.h"
#include "../../utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compraFinalizada = false;


static void imprimirCabecera() {
    printf("\n+------+---------------------------+--------+---------------+\n");
    printf("| %-4s | %-25s | %-6s | %-13s |\n",
           "PROD", "NOMBRE", "CANT.", "P.UNIT");
    printf("+------+---------------------------+--------+---------------+\n");
}

static void imprimirLinea(tCartItem item) {
    printf("| %-4s | %-25s | %-6d | $%-12.2f |\n",
           item.productId, item.name, item.quantity, item.unitPrice);
}

static void imprimirPie() {
    printf("+------+---------------------------+--------+---------------+\n");
}

static void imprimirItems(tCart* carrito) {
    imprimirCabecera();

    if (!carrito->items) {
        printf("| %-57s |\n", "Carrito vacio.");
        imprimirPie();
        return;
    }

    tCartItem* actual = carrito->items;
    while (actual) {
        imprimirLinea(*actual);
        actual = actual->next;
    }

    imprimirPie();
    printf("\nTOTAL: $%.2f\n", carrito->total);
}

static void crearMenu() {
    printf("\n[1] Agregar producto\n");
    printf("[2] Eliminar producto\n");
    printf("[3] Editar cantidad de un producto\n");
    printf("[4] Finalizar compra\n");
    printf("[x] Cancelar compra\n");
    printf("Seleccione una opcion: ");
}

void menuCarrito() {
    tUsuario* usuario = getUsuarioActual();

    if (usuario == NULL) {
        printf("\nError: No hay ningun usuario logueado.\n");
        printf("Debe iniciar sesion antes de realizar una compra.\n");
        return;
    }

    tCart carrito;
    generateCode(carrito.id);
    carrito.items = NULL;
    carrito.total = 0.0;
    strcpy(carrito.userId, usuario->id); 

    char opcion;
    do {
        printf("\n\t\t\t *** CARRITO: %s ***\n", carrito.id);
        imprimirItems(&carrito);
        crearMenu();

        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            menuAgregarAlCarrito(&carrito);
            break;
        case '2':
            menuEliminarEnCarrito(&carrito);
            break;
        case '3':
            menuEditarCantProductoEnCarrito(&carrito);
            break;
        case '4':
            menuPasarPorCaja(&carrito);
            //funcion de finalizar compra
            break;
        case 'x':
            printf("Cancelando compra...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 'x' && !compraFinalizada);
    
    liberarCarrito(&carrito);
    printf("Carrito liberado. Volviendo al menu de pedidos...\n");
}