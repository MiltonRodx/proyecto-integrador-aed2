#include "../categories/functions.h"
#include "../../utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCAMPO 50
#define MAXLINEA 300
#define MAXREGISTROS 100
#define ARCHIVO "build/stock/products/products.csv"

typedef char tString[MAXCAMPO];

typedef struct {
    tString id;
    tString name;
    tString code;
    tString stock;
    tString price;
    tString category;
    tString createdAt;
    tString updatedAt;
} tProducto;

void parsearLinea(char* linea, tProducto* producto) {
    char* token = strtok(linea, ",");
    if (token)
        strcpy(producto->id, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->name, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->code, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->stock, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->price, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->category, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->createdAt, token);

    token = strtok(NULL, ",");
    if (token)
        strcpy(producto->updatedAt, token);
}

int idExiste(const char* id) {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        return 0;
    }

    char linea[MAXLINEA];
    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.id, id) == 0) {
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);
    return 0;
}

int codigoExiste(const char* code) {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        return 0;
    }

    char linea[MAXLINEA];
    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.code, code) == 0) {
            fclose(archivo);
            return 1;
        }
    }

    fclose(archivo);
    return 0;
}

void imprimirCabecera() {
    printf("\n%-5s %-20s %-10s %-8s %-10s %-15s %-20s %-20s\n",
           "ID", "NOMBRE", "CODIGO", "STOCK", "PRECIO", "CATEGORIA", "CREADO", "ACTUALIZADO");
    printf("------------------------------------------------------------------------------------------------------------\n");
}

void imprimirLinea(tProducto pProducto) {
    printf("%-5s %-20s %-10s %-8s %-10s %-15s %-20s %-20s\n",
           pProducto.id, pProducto.name, pProducto.code, pProducto.stock,
           pProducto.price, pProducto.category, pProducto.createdAt, pProducto.updatedAt);
}

void imprimirProductoEncontrado(tProducto pProducto) {
    printf("\nProducto encontrado:\n");
    printf("ID: %s\n", pProducto.id);
    printf("Nombre: %s\n", pProducto.name);
    printf("Codigo: %s\n", pProducto.code);
    printf("Stock: %s\n", pProducto.stock);
    printf("Precio: %s\n", pProducto.price);
    printf("Categoria: %s\n", pProducto.category);
    printf("Creado: %s\n", pProducto.createdAt);
    printf("Actualizado: %s\n", pProducto.updatedAt);
}

void imprimirMenuBusqueda() {
    printf("\n---| MENU DE BUSQUEDA |---\n");
    printf("[1] Por nombre\n");
    printf("[2] Por id\n");
    printf("[3] Por codigo\n");
    printf("[4] Por categoria\n");
    printf("[5] Por stock\n");
    printf("[6] Volver al menu principal\n");
    printf("Opcion: ");
}

void crearProducto() {
    FILE* archivo = fopen(ARCHIVO, "a");
    if (!archivo) {
        printf("Error: No se pudo abrir o crear el archivo.\n");
        return;
    }

    tProducto producto;

    printf("Ingrese ID del producto: ");
    fflush(stdin);
    scanf(" %49[^\n]", producto.id);

    if (idExiste(producto.id)) {
        printf("Error: El ID '%s' ya existe.\n", producto.id);
        fclose(archivo);
        return;
    }

    printf("Ingrese nombre del producto: ");
    fflush(stdin);
    scanf(" %49[^\n]", producto.name);
    formatearNombre(producto.name);

    // Crear código
    tString AUXCOD;
    strcpy(AUXCOD, producto.name);
    formatearCodigo(AUXCOD);
    strcpy(producto.code, AUXCOD);

    if (codigoExiste(producto.code)) {
        printf("Error: El codigo '%s' ya existe.\n", producto.code);
        fclose(archivo);
        return;
    }

    printf("Ingrese stock: ");
    fflush(stdin);
    scanf(" %49[^\n]", producto.stock);

    if (producto.stock < 0) {
        printf("Error: El stock no debe ser menor que 0.\n");
        fclose(archivo);
        return;
    }

    printf("Ingrese precio: ");
    fflush(stdin);
    scanf(" %49[^\n]", producto.price);

    if (producto.price < 0) {
        printf("Error: El precio no debe ser menor que 0.\n");
        fclose(archivo);
        return;
    }

    printf("Ingrese categoria: ");
    fflush(stdin);
    scanf(" %49[^\n]", producto.category);
    mayus(producto.category);

    if (!categoriaExiste(producto.category)) {
        printf("Error: La categoria '%s' no existe.\n", producto.category);
        fclose(archivo);
        return;
    }

    obtenerFechaHora(producto.createdAt);
    strcpy(producto.updatedAt, producto.createdAt);

    fprintf(archivo, "%s,%s,%s,%s,%s,%s,%s,%s\n",
            producto.id, producto.name, producto.code, producto.stock,
            producto.price, producto.category, producto.createdAt, producto.updatedAt);

    fclose(archivo);
    printf("Producto '%s' agregado exitosamente.\n", producto.name);
}

void crearProductos() {
  int salir = 0;
  do {
    crearProducto();
    printf("Desea agregar otra producto? 1-Si 2-No: ");
    scanf("%d", &salir);
  } while (salir == 1);
}

void leerProductos() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("Error: No se pudo abrir o crear el archivo.\n");
        return;
    }

    char linea[MAXLINEA];
    int count = 0;

    imprimirCabecera();

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        imprimirLinea(producto);
        count++;
    }

    fclose(archivo);

    if (count == 0) {
        printf("No hay productos en el archivo.\n");
    } else {
        printf("\nTotal de productos: %d\n", count);
    }
}

void buscarProductosPorNombre() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tString busqueda;
    printf("Ingrese el producto a buscar: ");
    fflush(stdin);
    scanf(" %49[^\n]", busqueda);

    if (strlen(busqueda) == 0) {
        printf("Error: La busqueda no puede estar vacia.\n");
        fclose(archivo);
        return;
    }

    mayus(busqueda);

    char linea[MAXLINEA];
    int totalCoincidencias = 0;

    imprimirCabecera();

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        tString nombreMayus;
        strcpy(nombreMayus, producto.name);
        mayus(nombreMayus);

        if (strstr(nombreMayus, busqueda) != NULL) {
            imprimirLinea(producto);
            totalCoincidencias++;
        }
    }

    fclose(archivo);

    if (totalCoincidencias == 0) {
        printf("No se encontraron productos que contengan '%s'.\n", busqueda);
    } else {
        printf("\nTotal de coincidencias: %d\n", totalCoincidencias);
    }
}

void buscarProductoPorID() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tString id;
    printf("Ingrese el ID del producto: ");
    fflush(stdin);
    scanf(" %49[^\n]", id);

    char linea[MAXLINEA];
    int encontrado = 0;

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.id, id) == 0) {
            imprimirProductoEncontrado(producto);
            encontrado = 1;
            break;
        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("Error: El producto con ID '%s' no se encontro.\n", id);
    }
}

void buscarProductoPorCodigo() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tString codigo;
    printf("Ingrese el codigo del producto: ");
    fflush(stdin);
    scanf(" %49[^\n]", codigo);

    char linea[MAXLINEA];
    int encontrado = 0;

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.code, codigo) == 0) {
            imprimirProductoEncontrado(producto);
            encontrado = 1;
            break;
        }
    }

    fclose(archivo);

    if (!encontrado) {
        printf("Error: El producto con codigo '%s' no se encontro.\n", codigo);
    }
}

void buscarPorCategoria() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    tString categoria;
    printf("Ingrese la categoria: ");
    fflush(stdin);
    scanf(" %49[^\n]", categoria);
    mayus(categoria);

    if (!categoriaExiste(categoria)) {
        printf("Error: La categoria '%s' no existe.\n", categoria);
        fclose(archivo);
        return;
    }

    char linea[MAXLINEA];
    int totalCoincidencias = 0;

    imprimirCabecera();

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.category, categoria) == 0) {
            imprimirLinea(producto);
            totalCoincidencias++;
        }
    }

    fclose(archivo);

    if (totalCoincidencias == 0) {
        printf("No se encontraron productos de la categoria '%s'.\n", categoria);
    } else {
        printf("\nTotal de productos: %d\n", totalCoincidencias);
    }
}

void buscarProductoPorStock() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    int stock;
    printf("Ingrese el stock del producto: ");
    int resultado = scanf("%d", &stock);

    if (resultado != 1) {
        printf("Entrada invalida. Por favor ingrese un numero.\n");
        return;
    }

    char linea[MAXLINEA];
    int totalCoincidencias = 0;

    imprimirCabecera();

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &producto);

        if (atoi(producto.stock) == stock) {
            imprimirLinea(producto);
            totalCoincidencias++;
        }
    }

    fclose(archivo);

    if (totalCoincidencias == 0) {
        printf("No se encontraron productos con stock: %d.\n", stock);
    } else {
        printf("\nTotal de productos: %d\n", totalCoincidencias);
    }
}

void menuBusqueda() {
    int opcion;
    int resultado;

    do {
        imprimirMenuBusqueda();

        resultado = scanf("%d", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case 1:
            buscarProductosPorNombre();
            break;
        case 2:
            buscarProductoPorID();
            break;
        case 3:
            buscarProductoPorCodigo();
            break;
        case 4:
            buscarPorCategoria();
            break;
        case 5:
            buscarProductoPorStock();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 6);
}

void eliminarProducto() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char lineas[MAXREGISTROS][MAXLINEA];
    int count = 0;

    while (count < MAXREGISTROS && fgets(lineas[count], MAXLINEA, archivo)) {
        lineas[count][strcspn(lineas[count], "\n")] = '\0';
        count++;
    }
    fclose(archivo);

    if (count == 0) {
        printf("No hay productos para eliminar.\n");
        return;
    }

    tString id;
    printf("Ingrese el ID del producto a eliminar: ");
    fflush(stdin);
    scanf(" %49[^\n]", id);

    int indiceEncontrado = -1;
    tProducto productoEliminar;

    for (int i = 0; i < count; i++) {
        tProducto producto;
        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, lineas[i]);
        parsearLinea(lineaCopia, &producto);

        if (strcmp(producto.id, id) == 0) {
            indiceEncontrado = i;
            productoEliminar = producto;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: El producto con ID '%s' no se encontro.\n", id);
        return;
    }

    archivo = fopen(ARCHIVO, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i != indiceEncontrado) {
            fprintf(archivo, "%s\n", lineas[i]);
        }
    }
    fclose(archivo);

    printf("Producto '%s' (ID: %s) eliminado exitosamente.\n",
           productoEliminar.name, productoEliminar.id);
}