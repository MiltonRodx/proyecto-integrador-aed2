#ifndef CARTS_H
#define CARTS_H
#include <stdbool.h>
#include "../../types.h"

extern bool compraFinalizada;
typedef struct tCartItem {
    tId productId;
    tString name;
    int quantity;
    float unitPrice;
    struct tCartItem* next;
} tCartItem;

typedef struct {
    tId id;
    tId userId;
    tCartItem* items;
    float total;
} tCart;

void menuAgregarAlCarrito(tCart* carrito);
void menuEliminarEnCarrito(tCart* carrito);
void menuEditarCantProductoEnCarrito(tCart* carrito);
void menuPasarPorCaja(tCart* carrito);

void calcularTotalCarrito(tCart* carrito);
void liberarCarrito(tCart* carrito);

void menuCarrito();


#endif