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

    if (!estaLoggeado) {
        return 0;
    }

    tUsuario* user = getUsuarioActual();
    printf("\nBienvenido, %s (%s)\n", user->fullName, user->role);

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
            printf("Menu de caja (por implementar)\n");
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

// #include "stock/stock.h"
// #include "users/users.h"
// #include "users/auth/auth.h"
// #include <stdio.h>
// #include <ctype.h>

// void crearMenuPrincipal() {
//     printf("\n===| MENU PRINCIPAL |===\n");
//     printf("[1] Users\n");
//     printf("[2] Stock\n");
//     printf("[3] Cash\n");
//     printf("[x] Salir\n");
//     printf("Opcion: ");
// }

// int main() {
//     char opcion;

//     do {
//         crearMenuPrincipal();
//         scanf(" %c", &opcion);
//         opcion = tolower(opcion);

//         switch (opcion) {
//         case '1':
//             printf("Entrando al menú de usuarios...\n");
//             menuUsuarios(); 
//             break;

//         case '2':
//             printf("Entrando al menú de stock...\n");
//             menuStock();
//             break;

//         case '3':
//             printf("Menu de caja\n");
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