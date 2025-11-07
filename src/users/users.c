#include "users.h"
#include <stdio.h>

void menuUsuarios() {
    char opc;

    do {
        printf("\n---| MENU DE USUARIOS |---\n");
        printf("[1] Crear\n");
        printf("[2] Listar\n");
        printf("[3] Buscar\n");
        printf("[4] Actualizar\n");
        printf("[5] Borrar\n");
        printf("[x] Opcion anterior\n");
        printf("Seleccione una opcion: ");
        scanf(" %c", &opc);

        switch (opc) {
        case '1':
            crearUsuario();
            break;
        case '2':
            listarUsuarios();
            break;
        case '3':
            menuBusquedaUsuarios();
            break;
        case '4':
            actualizarUsuario();
            break;
        case '5':
            eliminarUsuario();
            break;
        case 'x':
            printf("Volviendo al menu anterior...\n");
            break;
        default:
            printf("Opcion no valida.\n");
            break;
        }
    } while (opc != 'x');
}