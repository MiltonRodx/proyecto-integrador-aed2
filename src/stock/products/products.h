#ifndef PRODUCTS_H
#define PRODUCTS_H

#include "../../types.h"

#define MAXLINEA 300
#define MAXREGISTROS 100
#define ARCHIVO_PRODUCTOS "stock/products/products.csv"

typedef struct {
    tString id;
    tString name;
    tString code;
    int stock;
    float price;
    tString category;
    tString createdAt;
    tString updatedAt;
} tProducto;

typedef struct {
    tProducto* datos;
    int tam;
} tListaProductos;

tListaProductos leerArchivoProductos();

int idProductoExiste(tString);
int codigoExiste(tString);
tProducto* buscarProductoPorId(tString id);
int validarStockDisponible(tString id, int cantidad);

void crearProductos();
void obtenerProductos();
void menuBusquedaProductos();
void actualizarProducto();
void eliminarProducto();

void imprimirProductos(tListaProductos productos);
void imprimirProductoDetallado(tProducto producto);

void menuProductos();

#endif