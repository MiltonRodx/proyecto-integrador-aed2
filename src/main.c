#include "stock/stock.h"
#include "users/users.h"
#include "users/auth/auth.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


void crearMenuPrincipal() {
    printf("\n===| MENU PRINCIPAL |===\n");
    printf("[1] Users\n");
    printf("[2] Stock\n");
    printf("[3] Cash\n");
    printf("[x] Salir\n");
    printf("Opcion: ");
}

int main() {
    login();

    if (!estaLoggeado){
        return 0;
    }

    char opcion;

    do {
        crearMenuPrincipal();

        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            menuUsuarios();
            break;
        case '2':
            menuStock();
            break;
        case 'x':
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
            break;
        }

    } while (opcion != 'x');
    
    return 0;
}