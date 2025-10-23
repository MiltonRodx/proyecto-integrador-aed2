#include <stdio.h>
#include "categories.h"

void menuCategories() {
    int opcion;
    do {
        printf("\n--- MENU DE CATEGORIAS ---\n");
        printf("1. Agregar\n");
        printf("2. Listar\n");
        printf("3. Buscar\n");
        printf("4. Filtrar\n");
        printf("5. Eliminar\n");
        printf("6. Volver al menu principal\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Agregar categoria\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while (opcion != 6);
}