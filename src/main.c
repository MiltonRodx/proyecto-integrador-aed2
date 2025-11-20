#include "orders/orders.h"
#include "stock/stock.h"
#include "users/users.h"
#include <ctype.h>
#include <stdio.h>

void crearMenuPrincipal() {
    printf("\n===| MENU PRINCIPAL |===\n");
    printf("[1] Usuarios\n");
    printf("[2] Inventario\n");
    printf("[3] Pedidos\n");
    printf("[x] Salir\n");
    printf("Opcion: ");
}

int main() {
    login();

    if (!estaLoggeado) {
        return 0;
    }

    char opcion;

    do {
        crearMenuPrincipal();
        scanf(" %c", &opcion);
        opcion = tolower(opcion);

        switch (opcion) {
        case '1':
            if (esAdmin()) {
                menuUsuarios();
            } else {
                printf("No tienes permisos para acceder al menu de usuarios.\n");
            }
            break;

        case '2':
            if (esInventoryManager() || esAdmin()) {
                menuStock();
            } else {
                printf("No tienes permisos para acceder al menu de stock.\n");
            }
            break;

        case '3':
            if (esClient() || esAdmin()) {
                menuPedidos();
            } else {
                printf("No tienes permisos para acceder al menu de pedidos.\n");
            }
            break;

        case 'x':
            printf("Saliendo del programa...\n");
            break;

        default:
            printf("Opcion invalida. Intente nuevamente.\n");
            break;
        }

    } while (opcion != 'x');

    logout();
    return 0;
}

// #include "orders/orders.h"
// #include "stock/stock.h"
// #include "users/users.h"
// #include <ctype.h>
// #include <stdio.h>

// void crearMenuPrincipal() {
//     printf("\n===| MENU PRINCIPAL |===\n");
//     printf("[1] Users\n");
//     printf("[2] Stock\n");
//     printf("[3] Pedidos\n");
//     printf("[x] Salir\n");
//     printf("Seleccione una opcion: ");
// }

// int main() {
//     char opcion;

//     do {
//         crearMenuPrincipal();
//         scanf(" %c", &opcion);
//         opcion = tolower(opcion);

//         switch (opcion) {
//         case '1':
//             printf("Entrando al menu de usuarios...\n");
//             menuUsuarios();
//             break;

//         case '2':
//             printf("Entrando al menu de stock...\n");
//             menuStock();
//             break;

//         case '3':
//             printf("Entrando al menu de pedidos...\n");
//             menuPedidos();
//             break;

//         case 'x':
//             printf("Saliendo del programa...\n");
//             break;

//         default:
//             printf("Opcion invalida. Intente nuevamente.\n");
//             break;
//         }

//     } while (opcion != 'x');

//     return 0;
// }