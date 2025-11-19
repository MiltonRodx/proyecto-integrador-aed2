#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "../../types.h"

#define MAXLINEAS 100
#define MAXCATEGORIA 100
#define ARCHIVO_CATEGORIAS "stock/categories/categories.txt"

typedef struct {
    tString* datos;
    int tam;
} tListaCategorias;

void menuCategorias();

int categoriaExiste(const char*);
void leerCategorias();
void crearCategorias();
void filtrarCategoriaPorNombre();
void buscarCategorias();
void editarCategoria();
void eliminarCategoria();

#endif