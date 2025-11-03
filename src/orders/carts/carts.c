#include "carts.h"
#include "../../utils/utils.h"
#include "../users/auth/auth.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void imprimirCabecera() {
    printf("\n+------+------+---------------------------+--------+--------+\n");
    printf("| %-4s | %-4s | %-25s | %-6s | %-6s |\n",
           "ID", "USUARIO", "NOMBRE", "CANTIDAD", "PRECIO UNIT.");
    printf("\n+------+------+---------------------------+--------+--------+\n");
}

static void imprimirLinea(tCartItem item) {
    printf("| %-4s | %-4s | %-25s | %-6d | $%-6.2f |\n",
           item.id, item.productId, item.name, item.quantity, item.unitPrice);
}

static void imprimirPie() {
    printf("+------+------+---------------------------+--------+--------+\n");
}

static void imprimirItems(tCartItem* items) {
    imprimirCabecera();

    if (!items) {
        printf("| %-58s |\n", "Carrito vacío.");
        imprimirPie();
        return;
    }

    tCartItem* actual = items;
    while (actual) {
        imprimirLinea(*actual);
        actual = actual->next;
    }

    imprimirPie();
}

void comenzarCompra(int idUsuario) {
    tCart carrito;
    generateCode(carrito.id);
    carrito.items = NULL;

    printf("\t\t\t *** CARRITO: %d ***\n\n", carrito.id);
    imprimirItems(carrito.items);

    int opcion;
    do {
        printf("\n1. Agregar producto");
        printf("\n2. Eliminar producto");
        printf("\n3. Editar cantidad de un producto");
        printf("\n4. Finalizar compra");
        printf("\n5. Cancelar compra\n");
        printf("Seleccione: ");
        scanf("%d", &opcion);

        // switch (opcion) {
        // case 1:
        //     agregarProductoAlCarrito(&carrito);
        //     break;
        // case 2:
        //     eliminarProductoDelCarrito(&carrito);
        //     break;
        // case 3:
        //     editarCantidadProducto(&carrito);
        //     break;
        // case 4:
        //     pasarPorCaja(&carrito, idUsuario);
        //     break;
        // case 5:
        //     liberarCarrito(&carrito);
        //     printf("Compra cancelada.\n");
        //     return;
        // }
    } while (opcion != 4 && opcion != 5);
}
