#include "categories.h"
#include "functions.h"
#include <stdio.h>

void crearMenuCategorias() {
    printf("\n---| MENU DE CATEGORIAS |---\n");
    printf("[1] Agregar\n");
    printf("[2] Listar\n");
    printf("[3] Buscar\n");
    printf("[4] Filtrar por nombre\n");
    printf("[5] Editar\n");
    printf("[6] Eliminar\n");
    printf("[x] Volver al menu principal\n");
    printf("Seleccione una opcion: ");
}

void menuCategorias() {
    char opcion;
    int resultado;

    do {
        crearMenuCategorias();

        resultado = scanf(" %c", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            crearCategorias();
            break;
        case '2':
            leerCategorias();
            break;
        case '3':
            buscarCategorias();
            break;
        case '4':
            filtrarCategoriaPorNombre();
            break;
        case '5':
            editarCategoria();
            break;
        case '6':
            eliminarCategoria();
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}