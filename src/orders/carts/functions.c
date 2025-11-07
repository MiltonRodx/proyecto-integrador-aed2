#include "../../stock/stock.h"
#include "../../utils/utils.h"
#include "carts.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void calcularTotalCarrito(tCart* carrito) {
    float total = 0.0;
    tCartItem* actual = carrito->items;

    while (actual != NULL) {
        total += actual->quantity * actual->unitPrice;
        actual = actual->next;
    }

    carrito->total = total;
}

void liberarCarrito(tCart* carrito) {
    tCartItem* actual = carrito->items;

    while (actual != NULL) {
        tCartItem* siguiente = actual->next;
        free(actual);
        actual = siguiente;
    }

    carrito->items = NULL;
    carrito->total = 0.0;
}

static tCartItem* buscarItemEnCarrito(tCart* carrito, tString productId) {
    tCartItem* actual = carrito->items;

    while (actual != NULL) {
        if (strcmp(actual->productId, productId) == 0) {
            return actual;
        }
        actual = actual->next;
    }

    return NULL;
}

static void crearMenuAgregarAlCarrito() {
    printf("\n---| Agregar producto al carrito |---\n");
    printf("[1] Ingresar ID\n");
    printf("[x] Volver al menu\n");
    printf("Seleccione una opcion: ");
}

static void agregarProductoAlCarrito(tCart* carrito) {
    printf("\n=== PRODUCTOS DISPONIBLES ===\n");
    obtenerProductos();

    tString id;
    printf("\nIngrese el ID del producto a agregar: ");
    fflush(stdin);
    scanf(" %4[^\n]", id);
    mayus(id);

    if (!idProductoExiste(id)) {
        printf("Error: El producto con ID '%s' no existe.\n", id);
        return;
    }

    tCartItem* itemExistente = buscarItemEnCarrito(carrito, id);
    if (itemExistente) {
        printf("Error: El producto ya esta en el carrito. Use la opcion 'Editar cantidad' para modificarlo.\n");
        return;
    }

    tProducto* producto = buscarProductoPorId(id);

    if (!producto) {
        printf("Error al obtener informacion del producto.\n");
        return;
    }

    if (producto->stock <= 0) {
        printf("\nError: El producto no tiene stock disponible.\n");
        free(producto);
        return;
    }

    int cantidad;
    printf("\nIngrese la cantidad a agregar (Stock disponible: %d): ", producto->stock);
    if (scanf("%d", &cantidad) != 1 || cantidad <= 0) {
        printf("Error: Cantidad invalida.\n");
        free(producto);
        return;
    }

    if (cantidad > producto->stock) {
        printf("Error: Stock insuficiente. Stock disponible: %d\n", producto->stock);
        free(producto);
        return;
    }

    tCartItem* nuevoItem = malloc(sizeof(tCartItem));
    if (!nuevoItem) {
        printf("Error al asignar memoria.\n");
        free(producto);
        return;
    }

    strcpy(nuevoItem->productId, producto->id);
    strcpy(nuevoItem->name, producto->name);
    nuevoItem->quantity = cantidad;
    nuevoItem->unitPrice = producto->price;

    nuevoItem->next = carrito->items;
    carrito->items = nuevoItem;

    printf("\nProducto agregado al carrito exitosamente.\n");

    calcularTotalCarrito(carrito);

    free(producto);
}

void menuAgregarAlCarrito(tCart* carrito) {
    char opcion;

    do {
        crearMenuAgregarAlCarrito();
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            agregarProductoAlCarrito(carrito);
            break;
        case 'x':
            printf("Volviendo al menu del carrito...\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}

static void crearMenuEliminarEnCarrito() {
    printf("\n---| Eliminar producto del carrito |---\n");
    printf("[1] Ingresar ID\n");
    printf("[x] Volver al menu\n");
    printf("Seleccione una opcion: ");
}

static void eliminarProductoDelCarrito(tCart* carrito) {
    if (!carrito->items) {
        printf("\nEl carrito esta vacio. No hay productos para eliminar.\n");
        return;
    }

    tString productId;
    printf("\nIngrese el ID del producto a eliminar del carrito: ");
    fflush(stdin);
    scanf(" %4[^\n]", productId);
    mayus(productId);

    tCartItem* actual = carrito->items;
    tCartItem* anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->productId, productId) == 0) {
            if (anterior == NULL) {
                carrito->items = actual->next;
            } else {
                anterior->next = actual->next;
            }

            printf("\nProducto '%s' eliminado del carrito.\n", actual->name);
            free(actual);

            calcularTotalCarrito(carrito);
            return;
        }

        anterior = actual;
        actual = actual->next;
    }

    printf("\nError: No se encontro el producto con ID '%s' en el carrito.\n", productId);
}

void menuEliminarEnCarrito(tCart* carrito) {
    char opcion;

    do {
        crearMenuEliminarEnCarrito();
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            eliminarProductoDelCarrito(carrito);
            break;
        case 'x':
            printf("Volviendo al menu del carrito...\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}