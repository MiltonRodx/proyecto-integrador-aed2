#include "../../stock/stock.h"
#include "../../utils/utils.h"
#include "carts.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../stock/products/products.h"

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



static void crearMenuEditarCantProductoEnCarrito(tCart* carrito){
    printf("---| Editar cantidad de un producto |---\n");
    printf("[1] Modificar por ID\n");
    printf("[2] Modificar por nombre\n");
    printf("[x] Volver al menu\n");
    printf("Seleccione una opcion: ");
}

static void editarProductoEnCarritoPorID(tCart* carrito) {
    if (!carrito || !carrito->items) {
        printf("No hay productos en el carrito\n");
        return;
    }

    char auxId[64];
    printf("Inserte id a buscar: ");
    scanf(" %63[^\n]", auxId);

    tCartItem* actual = carrito->items;
    while (actual != NULL) {
        if (strcmp(actual->productId, auxId) == 0) {
            printf("Producto encontrado, cantidad actual: %d\n", actual->quantity);

            FILE* f = fopen("src/stock/products/products.csv", "r");
            if (!f) {
                printf("No se pudo abrir el archivo de productos.\n");
                return;
            }

            char linea[512];
            fgets(linea, sizeof(linea), f); // descartar cabecera

            tProducto reg;
            int stock = 0;
            int encontrado = 0;

            while (fgets(linea, sizeof(linea), f)) {
                char id[128], name[128], code[128], category[128], created[128], updated[128];
                float price;
                int st;

                sscanf(linea, "%[^,],%[^,],%[^,],%d,%f,%[^,],%[^,],%[^\n]",
                       id, name, code, &st, &price, category, created, updated);

                if (strcmp(id, auxId) == 0) {
                    stock = st;
                    encontrado = 1;
                    break;
                }
            }

            fclose(f);

            if (!encontrado) {
                printf("El producto no se encontró en el stock.\n");
                return;
            }

            int nuevaCant;
            do {
                printf("Inserte nueva cantidad (stock disponible: %d): ", stock);
                scanf("%d", &nuevaCant);
            } while (nuevaCant > stock || nuevaCant < 0);

            actual->quantity = nuevaCant;
            printf("Cantidad actualizada a %d.\n", actual->quantity);
            return;
        }

        actual = actual->next;
    }

    printf("Producto no encontrado en el carrito.\n");
}

static void editarProductoEnCarritoPorNombre(tCart* carrito) {
    if (!carrito || !carrito->items) {
        printf("No hay productos en el carrito\n");
        return;
    }

    char auxName[50];
    printf("Inserte nombre a buscar: ");
    scanf(" %49[^\n]", auxName);

    tCartItem* actual = carrito->items;
    while (actual != NULL) {
        if (strcmp(actual->productId, auxName) == 0) {
            printf("Producto encontrado, cantidad actual: %d\n", actual->quantity);

            FILE* f = fopen("src/stock/products/products.csv", "r");
            if (!f) {
                printf("No se pudo abrir el archivo de productos.\n");
                return;
            }

            char linea[512];
            fgets(linea, sizeof(linea), f); // descartar cabecera

            int stock = 0;
            int encontrado = 0;

            while (fgets(linea, sizeof(linea), f)) {
                char id[128], name[128], code[128], category[128], created[128], updated[128];
                float price;
                int st;

                sscanf(linea, "%[^,],%[^,],%[^,],%d,%f,%[^,],%[^,],%[^\n]",
                       id, name, code, &st, &price, category, created, updated);

                if (strcmp(name, auxName) == 0) {
                    stock = st;
                    encontrado = 1;
                    break;
                }
            }

            fclose(f);

            if (!encontrado) {
                printf("El producto no se encontró en el stock.\n");
                return;
            }

            int nuevaCant;
            do {
                printf("Inserte nueva cantidad (stock disponible: %d): ", stock);
                scanf("%d", &nuevaCant);
            } while (nuevaCant > stock || nuevaCant < 0);

            actual->quantity = nuevaCant;
            printf("Cantidad actualizada a %d.\n", actual->quantity);
            return;
        }

        actual = actual->next;
    }

    printf("Producto no encontrado en el carrito.\n");
}

void menuEditarCantProductoEnCarrito(tCart* carrito){
    char opc;

    do {
        crearMenuEditarProductoEnCarrito();
        scanf(" %c", &opc);
        opc = tolower(opc);

        switch (opc) {
        case '1':
            editarProductoEnCarritoPorID(carrito);
            break;
        case '2':
            editarProductoEnCarritoPorNombre(carrito);
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while (opc != 'x');
}