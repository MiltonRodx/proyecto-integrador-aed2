#ifndef CARTS_H
#define CARTS_H

// Constantes
#define MAX_LEN 64
#define MAX_ID 4

// Tipos
typedef char tString[MAX_LEN];
typedef char tId[MAX_ID];

// Estructura del item del carrito
typedef struct tCartItem {
    tId id;
    tId productId;
    tString name;
    int quantity;
    float unitPrice;
    struct tCartItem* next;
} tCartItem;

// Estructura del carrito
typedef struct {
    tId id;
    tId userId;
    tCartItem* items;
    float total;
} tCart;

#endif