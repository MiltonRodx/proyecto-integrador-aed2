#include <stdio.h>
#include "stock/stock.h"

int main() {
    int opcion;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Users\n");
        printf("2. Stock\n");
        printf("4. Cash\n");
        printf("5. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 2:
                menuStock();
                break;
            case 5:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while (opcion != 5);

    return 0;
}