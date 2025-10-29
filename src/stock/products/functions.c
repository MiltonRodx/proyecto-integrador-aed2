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
    int stock;
    float price;
    tString category;
    tString createdAt;
    tString updatedAt;
} tProducto;

typedef struct {
    tProducto* datos;
    int tam;
} tLista;

void ingresarCampo(const char* mensaje, tString campo) {
    printf("%s", mensaje);
    fflush(stdin);
    scanf(" %49[^\n]", campo);
}

void parsearLinea(char* linea, tProducto* producto) {
    char* valor = strtok(linea, ",");

    if (valor)
        strcpy(producto->id, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(producto->name, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(producto->code, valor);

    valor = strtok(NULL, ",");
    if (valor)
        producto->stock = atoi(valor);

    valor = strtok(NULL, ",");
    if (valor)
        producto->price = atof(valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(producto->category, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(producto->createdAt, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(producto->updatedAt, valor);
}

static tLista leerArchivo() {
    FILE* archivo = fopen(ARCHIVO, "r");
    if (!archivo) {
        return (tLista){NULL, 0};
    }

    tLista lista = {NULL, 0};
    char linea[MAXLINEA];
    int esPrimeraLinea = 1;

    while (fgets(linea, MAXLINEA, archivo)) {
        linea[strcspn(linea, "\n")] = '\0';

        if (esPrimeraLinea) {
            esPrimeraLinea = 0;
            if (strstr(linea, "id,") != NULL) {
                continue;
            }
        }

        lista.datos = realloc(lista.datos, (lista.tam + 1) * sizeof(tProducto));
        if (!lista.datos) {
            printf("Error al reasignar memoria\n");
            fclose(archivo);
            return (tLista){NULL, 0};
        }

        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearLinea(lineaCopia, &lista.datos[lista.tam]);
        lista.tam++;
    }

    fclose(archivo);
    return lista;
}

int idExiste(tString valor) {
    tLista productos = leerArchivo();

    if (!productos.datos) {
        return 0;
    }

    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].id, valor) == 0) {
            free(productos.datos);
            return 1;
        }
    }

    free(productos.datos);
    return 0;
}

int codigoExiste(tString valor) {
    tLista productos = leerArchivo();

    if (!productos.datos) {
        return 0;
    }

    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].code, valor) == 0) {
            free(productos.datos);
            return 1;
        }
    }

    free(productos.datos);
    return 0;
}

static int escribirArchivo(tLista productos) {
    FILE* archivo = fopen(ARCHIVO, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        return 0;
    }

    for (int i = 0; i < productos.tam; i++) {
        fprintf(archivo, "%s,%s,%s,%d,%.2f,%s,%s,%s\n",
                productos.datos[i].id,
                productos.datos[i].name,
                productos.datos[i].code,
                productos.datos[i].stock,
                productos.datos[i].price,
                productos.datos[i].category,
                productos.datos[i].createdAt,
                productos.datos[i].updatedAt);
    }

    fclose(archivo);
    return 1;
}

void crearProducto() {
    tProducto producto;

    ingresarCampo("Ingrese el ID del producto: ", producto.id);
    if (idExiste(producto.id)) {
        printf("Error: El ID '%s' ya existe.\n", producto.id);
        return;
    }

    ingresarCampo("Ingrese el nombre del producto: ", producto.name);
    formatearNombre(producto.name);

    tString auxCod;
    strcpy(auxCod, producto.name);
    formatearCodigo(auxCod);
    strcpy(producto.code, auxCod);

    if (codigoExiste(producto.code)) {
        printf("Error: El codigo '%s' ya existe.\n", producto.code);
        return;
    }

    printf("Ingrese el stock: ");
    if (scanf("%d", &producto.stock) != 1 || producto.stock < 0) {
        printf("Error: Valor incorrecto.\n");
        return;
    }

    printf("Ingrese el precio: ");
    if (scanf("%f", &producto.price) != 1 || producto.price <= 0) {
        printf("Error: Valor incorrecto.\n");
        return;
    }

    ingresarCampo("Ingrese categoria: ", producto.category);
    mayus(producto.category);

    if (!categoriaExiste(producto.category)) {
        printf("Error: La categoria '%s' no existe.\n", producto.category);
        return;
    }

    obtenerFechaHora(producto.createdAt);
    strcpy(producto.updatedAt, producto.createdAt);

    tLista productos = leerArchivo();
    productos.datos = realloc(productos.datos, (productos.tam + 1) * sizeof(tProducto));
    if (!productos.datos) {
        printf("Error al agregar el producto.\n");
        return;
    }

    productos.datos[productos.tam] = producto;
    productos.tam++;

    if (escribirArchivo(productos)) {
        printf("Producto '%s' agregado exitosamente.\n", producto.name);
    }

    free(productos.datos);
}

void crearProductos() {
    int salir = 0;
    do {
        crearProducto();
        printf("Desea agregar otro producto? 1-Si 2-No: ");
        scanf("%d", &salir);
    } while (salir == 1);
}

void imprimirCabecera() {
    printf("\n%-4s %-25s %-20s %-8s %-12s %-22s %-20s %-20s\n",
           "ID", "NOMBRE", "CODIGO", "STOCK", "PRECIO", "CATEGORIA", "CREADO", "ACTUALIZADO");
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
}

void imprimirLinea(tProducto producto) {
    printf("%-4s %-25s %-20s %-8d $%-11.2f %-22s %-20s %-20s\n",
           producto.id, producto.name, producto.code,
           producto.stock, producto.price,
           producto.category, producto.createdAt, producto.updatedAt);
}

void imprimirProductos(tLista productos) {
    if (productos.tam == 0) {
        printf("No hay productos para mostrar.\n");
        return;
    }

    imprimirCabecera();
    for (int i = 0; i < productos.tam; i++) {
        imprimirLinea(productos.datos[i]);
    }
    printf("\nTotal de productos: %d\n", productos.tam);
}

void obtenerProductos() {
    tLista productos = leerArchivo();

    if (!productos.datos) {
        printf("Error: No se pudo abrir el archivo, tal vez no exista aun.\n");
        return;
    }

    imprimirProductos(productos);
    free(productos.datos);
}

void imprimirProductoDetallado(tProducto producto) {
    printf("\nProducto encontrado:\n");
    printf("ID: %s\n", producto.id);
    printf("Nombre: %s\n", producto.name);
    printf("Codigo: %s\n", producto.code);
    printf("Stock: %d\n", producto.stock);
    printf("Precio: $%.2f\n", producto.price);
    printf("Categoria: %s\n", producto.category);
    printf("Creado: %s\n", producto.createdAt);
    printf("Actualizado: %s\n", producto.updatedAt);
}

void filtrarProductoPorId() {
    tLista productos = leerArchivo();
    int cont = 0;

    tString id;

    ingresarCampo("Ingrese el ID del producto: ", id);
    if (!idExiste(id)) {
        printf("Error: El producto con el id: '%s' no existe.\n", id);
        return;
    }

    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].id, id) == 0) {
            imprimirProductoDetallado(productos.datos[i]);
            cont++;
        }
    }

    if (cont == 0) {
        printf("No se encontraron productos con el id '%s'.\n", id);
    }
}

void filtrarProductoPorCodigo() {
    tLista productos = leerArchivo();
    int cont = 0;

    tString codigo;

    ingresarCampo("Ingrese el codigo del producto: ", codigo);
    if (!codigoExiste(codigo)) {
        printf("Error: El producto con el id '%s' no existe.\n", codigo);
        return;
    }

    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].code, codigo) == 0) {
            imprimirProductoDetallado(productos.datos[i]);
            cont++;
        }
    }

    if (cont == 0) {
        printf("No se encontraron productos con el codigo '%s'.\n", codigo);
    }
}

void buscarProductosPorNombre() {
    tString busqueda;
    ingresarCampo("Ingrese el producto a buscar: ", busqueda);

    if (strlen(busqueda) == 0) {
        printf("Error: La busqueda no puede estar vacia.\n");
        return;
    }

    tLista productos = leerArchivo();

    if (!productos.datos) {
        printf("Error: No se pudo leer el archivo.\n");
        return;
    }

    tLista resultado = {NULL, 0};

    tString busquedaMayus;
    strcpy(busquedaMayus, busqueda);
    mayus(busquedaMayus);

    for (int i = 0; i < productos.tam; i++) {
        tString nombreMayus;
        strcpy(nombreMayus, productos.datos[i].name);
        mayus(nombreMayus);

        if (strstr(nombreMayus, busquedaMayus) != NULL) {
            resultado.datos = realloc(resultado.datos, (resultado.tam + 1) * sizeof(tProducto));
            if (!resultado.datos) {
                printf("Error al asignar memoria.\n");
                free(productos.datos);
                return;
            }
            resultado.datos[resultado.tam] = productos.datos[i];
            resultado.tam++;
        }
    }

    if (resultado.tam == 0) {
        printf("No se encontraron productos que contengan '%s'.\n", busqueda);
    } else {
        imprimirProductos(resultado);
    }

    free(productos.datos);
    free(resultado.datos);
}

void filtrarProductoPorCategoria() {
    tLista productos = leerArchivo();
    int cont = 0;

    tString categoria;

    ingresarCampo("Ingrese la categoria del producto: ", categoria);
    mayus(categoria);
    if (!categoriaExiste(categoria)) {
        printf("Error: La categoria: '%s' no existe.\n", categoria);
        return;
    }
    imprimirCabecera();
    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].category, categoria) == 0) {
            imprimirLinea(productos.datos[i]);
            cont++;
        }
    }

    if (cont == 0) {
        printf("No se encontraron productos con la categoria '%s'.\n", categoria);
    }
}

void filtrarProductoPorStock() {
    tLista productos = leerArchivo();
    int cont = 0;

    int stock;

    printf("Ingrese el stock: ");
    if (scanf("%d", &stock) != 1 || stock < 0) {
        printf("Error: Valor incorrecto.\n");
        return;
    }

    imprimirCabecera();
    for (int i = 0; i < productos.tam; i++) {
        if (productos.datos[i].stock == stock) {
            imprimirLinea(productos.datos[i]);
            cont++;
        }
    }

    if (cont == 0) {
        printf("No se encontraron productos con el stock '%d'.\n", stock);
    }
}

void imprimirMenuBusqueda() {
    printf("\n---| MENU DE BUSQUEDA |---\n");
    printf("[1] Por id\n");
    printf("[2] Por codigo\n");
    printf("[3] Por nombre\n");
    printf("[4] Por categoria\n");
    printf("[5] Por stock\n");
    printf("[x] Volver al menu principal\n");
    printf("Opcion: ");
}

void menuBusqueda() {
    char opcion;
    int resultado;

    do {
        imprimirMenuBusqueda();
        resultado = scanf(" %c", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            filtrarProductoPorId();
            break;
        case '2':
            filtrarProductoPorCodigo();
            break;
        case '3':
            buscarProductosPorNombre();
            break;
        case '4':
            filtrarProductoPorCategoria();
            break;
        case '5':
            filtrarProductoPorStock();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}

void actualizarProducto() {
    tLista productos = leerArchivo();

    if (!productos.datos || productos.tam == 0) {
        printf("No hay productos para editar.\n");
        free(productos.datos);
        return;
    }

    tString id;
    ingresarCampo("Ingrese el id del producto a editar: ", id);

    int indiceEncontrado = -1;
    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].id, id) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: El producto con ID '%s' no se encontró.\n", id);
        free(productos.datos);
        return;
    }

    printf("\nProducto actual:\n");
    imprimirProductoDetallado(productos.datos[indiceEncontrado]);

    tProducto productoEditado = productos.datos[indiceEncontrado];

    char opcion;
    do {
        printf("\n---| CAMPOS A EDITAR |---\n");
        printf("[1] Nombre\n");
        printf("[2] Stock\n");
        printf("[3] Precio\n");
        printf("[4] Categoria\n");
        printf("[x] Terminar edicion\n");
        printf("Opcion: ");
        if (scanf(" %c", &opcion) != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            ingresarCampo("Ingrese el nuevo nombre del producto: ", productoEditado.name);
            formatearNombre(productoEditado.name);

            tString auxCod;
            strcpy(auxCod, productoEditado.name);
            formatearCodigo(auxCod);

            if (codigoExiste(auxCod) && strcmp(auxCod, productoEditado.code) != 0) {
                printf("Error: El codigo '%s' ya existe. Intente con otro nombre.\n", auxCod);
                strcpy(productoEditado.name, productos.datos[indiceEncontrado].name);
            } else {
                strcpy(productoEditado.code, auxCod);
                printf("Nombre y codigo actualizados correctamente.\n");
            }
            break;

        case '2':
            printf("Ingrese el nuevo stock: ");
            int nuevoStock;
            if (scanf("%d", &nuevoStock) != 1 || nuevoStock < 0) {
                printf("Error: Valor incorrecto.\n");
            } else {
                productoEditado.stock = nuevoStock;
                printf("Stock actualizado correctamente.\n");
            }
            break;

        case '3':
            printf("Ingrese el nuevo precio: ");
            float nuevoPrecio;
            if (scanf("%f", &nuevoPrecio) != 1 || nuevoPrecio <= 0) {
                printf("Error: Valor incorrecto.\n");
            } else {
                productoEditado.price = nuevoPrecio;
                printf("Precio actualizado correctamente.\n");
            }
            break;

        case '4':
            ingresarCampo("Ingrese la nueva categoria: ", productoEditado.category);
            mayus(productoEditado.category);
            if (!categoriaExiste(productoEditado.category)) {
                printf("Error: La categoria '%s' no existe.\n", productoEditado.category);
                strcpy(productoEditado.category, productos.datos[indiceEncontrado].category);
            } else {
                printf("Categoria actualizada correctamente.\n");
            }
            break;

        case '5':
            break;

        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }

        if (opcion != 5) {
            printf("\nProducto actualizado (temporal):\n");
            imprimirProductoDetallado(productoEditado);
        }

    } while (opcion != 'x');

    obtenerFechaHora(productoEditado.updatedAt);

    productos.datos[indiceEncontrado] = productoEditado;

    if (escribirArchivo(productos)) {
        printf("\nEl producto con id: %s fue actualizado exitosamente.\n", id);
    } else {
        printf("\nError al guardar los cambios en el archivo.\n");
    }

    free(productos.datos);
}

void eliminarProducto() {
    tString id;
    ingresarCampo("Ingrese el ID del producto a eliminar: ", id);

    tLista productos = leerArchivo();

    if (!productos.datos || productos.tam == 0) {
        printf("No hay productos para eliminar.\n");
        return;
    }

    int indiceEncontrado = -1;
    tProducto productoEliminar;

    for (int i = 0; i < productos.tam; i++) {
        if (strcmp(productos.datos[i].id, id) == 0) {
            indiceEncontrado = i;
            productoEliminar = productos.datos[i];
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: El producto con id: %s no se encontro.\n", id);
        free(productos.datos);
        return;
    }

    tLista nuevaLista = {NULL, 0};
    nuevaLista.datos = malloc((productos.tam - 1) * sizeof(tProducto));

    if (!nuevaLista.datos && productos.tam > 1) {
        printf("Error de memoria.\n");
        free(productos.datos);
        return;
    }

    for (int i = 0; i < productos.tam; i++) {
        if (i != indiceEncontrado) {
            nuevaLista.datos[nuevaLista.tam] = productos.datos[i];
            nuevaLista.tam++;
        }
    }

    if (escribirArchivo(nuevaLista)) {
        printf("El producto con el id: %s, fue eliminado con exito.\n", productoEliminar.id);
    }

    free(productos.datos);
    free(nuevaLista.datos);
}