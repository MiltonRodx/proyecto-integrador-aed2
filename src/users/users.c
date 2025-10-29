#include "users.h"
#include "functions.h"
#include <stdio.h>

void menuUsuarios() {
    char opc;

    do {
        printf("\n=== Menu users: Manejo de Usuarios ===\n");
        printf("1. Crear usuario\n");
        printf("2. Leer usuario\n");
        printf("3. Actualizar usuario\n");
        printf("4. Borrar usuario\n");
        printf("x. Opcion anterior\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opc);

        switch (opc) {
            case '1':
                crearUsuario();
                break;
            case '2':
                menuLectura();
                break;
            case '3':
                actualizarUsuario();
                break;
            case '4':
                borrarUsuario();
                break;
            case '5':
                printf("Volviendo al menu anterior...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }
    } while (opc != 'x');
}