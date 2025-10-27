#include "stock/stock.h"
#include <stdio.h>

void crearMenuPrincipal() {
    printf("\n===| MENU PRINCIPAL |===\n");
    printf("[1] Users\n");
    printf("[2] Stock\n");
    printf("[4] Cash\n");
    printf("[5] Salir\n");
    printf("Opcion: ");
}

int main() {
    int opcion;
    int resultado;

    do {
        crearMenuPrincipal();

        resultado = scanf("%d", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case 2:
            menuStock();
            break;
        case 5:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while (opcion != 5);

    return 0;
}