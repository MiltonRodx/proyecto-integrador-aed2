// Librerias
#include "functions.h"
#include "../utils/utils.h"
#include "users.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definición de variables
tString roles[CANT_ROLE] = {"ADMIN", "INVENTORY_MANAGER", "CASHIER", "CLIENT"};

// Funciones
static void ingresarCampo(const char* mensaje, tString campo) {
    printf("%s", mensaje);
    fflush(stdin);
    scanf(" %49[^\n]", campo);
}

void parsearUsuario(char* linea, tUsuario* usuario) {
    char* valor = strtok(linea, ",");

    if (valor)
        strcpy(usuario->id, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->email, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->password, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->fullName, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->role, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->createdAt, valor);

    valor = strtok(NULL, ",");
    if (valor)
        strcpy(usuario->updatedAt, valor);
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

        lista.datos = realloc(lista.datos, (lista.tam + 1) * sizeof(tUsuario));
        if (!lista.datos) {
            printf("Error al reasignar memoria\n");
            fclose(archivo);
            return (tLista){NULL, 0};
        }

        char lineaCopia[MAXLINEA];
        strcpy(lineaCopia, linea);
        parsearUsuario(lineaCopia, &lista.datos[lista.tam]);
        lista.tam++;
    }

    fclose(archivo);
    return lista;
}

static int idExiste(tString valor) {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        return 0;
    }

    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].id, valor) == 0) {
            free(usuarios.datos);
            return 1;
        }
    }

    free(usuarios.datos);
    return 0;
}

void generarIdUnica(char* id) {
    do {
        generateCode(id);
    } while (idExiste(id));
}

int emailExiste(tString valor) {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        return 0;
    }

    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].email, valor) == 0) {
            free(usuarios.datos);
            return 1;
        }
    }

    free(usuarios.datos);
    return 0;
}

static int escribirArchivo(tLista usuarios) {
    FILE* archivo = fopen(ARCHIVO, "w");
    if (!archivo) {
        printf("Error: No se pudo abrir el archivo para escribir.\n");
        return 0;
    }

    fprintf(archivo, "id,email,password,fullName,rol,createdAt,updatedAt\n");

    for (int i = 0; i < usuarios.tam; i++) {
        fprintf(archivo, "%s,%s,%s,%s,%s,%s,%s\n",
                usuarios.datos[i].id,
                usuarios.datos[i].email,
                usuarios.datos[i].password,
                usuarios.datos[i].fullName,
                usuarios.datos[i].role,
                usuarios.datos[i].createdAt,
                usuarios.datos[i].updatedAt);
    }

    fclose(archivo);
    return 1;
}

int rolValido(tString role) {
    mayus(role);
    for (int i = 0; i < 4; i++) {
        if (strcmp(role, roles[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void imprimirOpcionesRoles() {
    printf("Seleccione un rol: \n");
    for (int i = 0; i < CANT_ROLE; i++) {
        printf("[%d] %s\n", i + 1, roles[i]);
    }
    printf("Seleccione una opcion: ");
}

void seleccionarRol(char* role) {
    int opcion;
    int resultado;
    int valido = 0;

    do {
        imprimirOpcionesRoles();

        resultado = scanf("%d", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            fflush(stdin);
            continue;
        }

        if (opcion >= 1 && opcion <= CANT_ROLE) {
            strcpy(role, roles[opcion - 1]);
            printf("Rol seleccionado: %s\n", role);
            valido = 1;
        } else {
            printf("Opcion invalida. Intente nuevamente.\n");
        }

    } while (!valido);
}

void crearUsuario() {
    tUsuario usuario;

    generarIdUnica(usuario.id);

    do {
        ingresarCampo("Ingrese email: ", usuario.email);
        minus(usuario.email);
        if (emailExiste(usuario.email)) {
            printf("Error: El email '%s' ya existe. Intente con otro.\n", usuario.email);
        }
    } while (emailExiste(usuario.email));

    ingresarCampo("Ingrese password: ", usuario.password);
    ingresarCampo("Ingrese nombre completo: ", usuario.fullName);
    formatearNombre(usuario.fullName);

    seleccionarRol(usuario.role);

    obtenerFechaHora(usuario.createdAt);
    strcpy(usuario.updatedAt, usuario.createdAt);

    tLista usuarios = leerArchivo();
    usuarios.datos = realloc(usuarios.datos, (usuarios.tam + 1) * sizeof(tUsuario));
    if (!usuarios.datos) {
        printf("Error al agregar el usuario.\n");
        return;
    }

    usuarios.datos[usuarios.tam] = usuario;
    usuarios.tam++;

    if (escribirArchivo(usuarios)) {
        printf("Usuario '%s' creado exitosamente con ID: %s\n", usuario.fullName, usuario.id);
    }

    free(usuarios.datos);
}

void crearUsuarios() {
    int salir = 0;
    do {
        crearUsuario();
        printf("¿Desea agregar otro usuario? 1-Si 2-No: ");
        scanf("%d", &salir);
    } while (salir == 1);
}

static void imprimirCabecera() {
    printf("\n+------+----------------------------+--------------------------------+--------------------+---------------------+---------------------+\n");
    printf("| %-4s | %-26s | %-30s | %-18s | %-19s | %-19s |\n",
           "ID", "EMAIL", "NOMBRE COMPLETO", "ROL", "CREADO", "ACTUALIZADO");
    printf("+------+----------------------------+--------------------------------+--------------------+---------------------+---------------------+\n");
}

static void imprimirLinea(tUsuario usuario) {
    printf("| %-4s | %-26s | %-30s | %-18s | %-19s | %-19s |\n",
           usuario.id, usuario.email,
           usuario.fullName, usuario.role,
           usuario.createdAt, usuario.updatedAt);
}

static void imprimirPie() {
    printf("+------+----------------------------+--------------------------------+--------------------+---------------------+---------------------+\n");
}

void imprimirUsuarios(tLista usuarios) {
    if (usuarios.tam == 0) {
        printf("No hay usuarios para mostrar.\n");
        return;
    }

    imprimirCabecera();
    for (int i = 0; i < usuarios.tam; i++) {
        imprimirLinea(usuarios.datos[i]);
    }
    imprimirPie();
    printf("\nTotal de usuarios: %d\n", usuarios.tam);
}

void obtenerUsuarios() {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    imprimirUsuarios(usuarios);
    free(usuarios.datos);
}

void imprimirUsuarioDetallado(tUsuario usuario) {
    printf("\nUsuario encontrado:\n");
    printf("ID: %s\n", usuario.id);
    printf("Email: %s\n", usuario.email);
    // printf("Password: %s\n", usuario.password);
    printf("Nombre Completo: %s\n", usuario.fullName);
    printf("Rol: %s\n", usuario.role);
    printf("Creado: %s\n", usuario.createdAt);
    printf("Actualizado: %s\n", usuario.updatedAt);
}

void buscarUserPorID() {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    tString id;
    ingresarCampo("Ingrese el ID del usuario: ", id);
    mayus(id);

    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].id, id) == 0) {
            imprimirUsuarioDetallado(usuarios.datos[i]);
            free(usuarios.datos);
            return;
        }
    }

    printf("Usuario con ID '%s' no encontrado.\n", id);
    free(usuarios.datos);
}

void buscarPorEmail() {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    tString email;
    ingresarCampo("Ingrese el email: ", email);
    minus(email);

    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].email, email) == 0) {
            imprimirUsuarioDetallado(usuarios.datos[i]);
            free(usuarios.datos);
            return;
        }
    }

    printf("Usuario con email '%s' no encontrado.\n", email);
    free(usuarios.datos);
}

void buscarPorNombre() {
    tString busqueda;
    ingresarCampo("Ingrese el nombre a buscar: ", busqueda);

    if (strlen(busqueda) == 0) {
        printf("Error: La busqueda no puede estar vacia.\n");
        return;
    }

    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        printf("Error: No se pudo leer el archivo.\n");
        return;
    }

    tLista resultado = {NULL, 0};

    tString busquedaMayus;
    strcpy(busquedaMayus, busqueda);
    mayus(busquedaMayus);

    for (int i = 0; i < usuarios.tam; i++) {
        tString nombreMayus;
        strcpy(nombreMayus, usuarios.datos[i].fullName);
        mayus(nombreMayus);

        if (strstr(nombreMayus, busquedaMayus) != NULL) {
            resultado.datos = realloc(resultado.datos, (resultado.tam + 1) * sizeof(tUsuario));
            if (!resultado.datos) {
                printf("Error al asignar memoria.\n");
                free(usuarios.datos);
                return;
            }
            resultado.datos[resultado.tam] = usuarios.datos[i];
            resultado.tam++;
        }
    }

    if (resultado.tam == 0) {
        printf("No se encontraron usuarios que contengan '%s'.\n", busqueda);
    } else {
        imprimirUsuarios(resultado);
    }

    free(usuarios.datos);
    free(resultado.datos);
}

void listarPorRol() {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos) {
        printf("Error: No se pudo abrir el archivo.\n");
        return;
    }

    tString role;
    seleccionarRol(role);

    if (!rolValido(role)) {
        printf("Error: El rol '%s' no es valido.\n", role);
        free(usuarios.datos);
        return;
    }

    int cont = 0;
    imprimirCabecera();
    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].role, role) == 0) {
            imprimirLinea(usuarios.datos[i]);
            cont++;
        }
    }
    imprimirPie();

    if (cont == 0) {
        printf("No se encontraron usuarios con el rol '%s'.\n", role);
    } else {
        printf("\nTotal de usuarios con rol '%s': %d\n", role, cont);
    }

    free(usuarios.datos);
}

void imprimirMenuBusquedaUsuarios() {
    printf("\n---| MENU DE BUSQUEDA |---\n");
    printf("[1] Por ID\n");
    printf("[2] Por email\n");
    printf("[3] Por nombre\n");
    printf("[x] Volver\n");
    printf("Seleccione una opcion: ");
}

void menuBusquedaUsuarios() {
    char opcion;
    int resultado;

    do {
        imprimirMenuBusquedaUsuarios();
        resultado = scanf(" %c", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese una opcion valida.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            buscarUserPorID();
            break;
        case '2':
            buscarPorEmail();
            break;
        case '3':
            buscarPorNombre();
            break;
        case 'x':
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}

void imprimirMenuListarUsuarios() {
    printf("\n---| MENU DE LISTADO |---\n");
    printf("[1] Listar todos\n");
    printf("[2] Listar por rol\n");
    printf("[x] Volver\n");
    printf("Seleccione una opcion: ");
}

void listarUsuarios() {
    char opcion;
    int resultado;

    do {
        imprimirMenuListarUsuarios();
        resultado = scanf(" %c", &opcion);

        if (resultado != 1) {
            printf("Entrada invalida. Por favor ingrese una opcion valida.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1':
            obtenerUsuarios();
            break;
        case '2':
            listarPorRol();
            break;
        case 'x':
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 'x');
}

void actualizarUsuario() {
    tLista usuarios = leerArchivo();

    if (!usuarios.datos || usuarios.tam == 0) {
        printf("No hay usuarios para editar.\n");
        free(usuarios.datos);
        return;
    }

    tString id;
    ingresarCampo("Ingrese el ID del usuario a editar: ", id);
    mayus(id);

    int indiceEncontrado = -1;
    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].id, id) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: El usuario con ID '%s' no se encontro.\n", id);
        free(usuarios.datos);
        return;
    }

    printf("\nUsuario actual:\n");
    imprimirUsuarioDetallado(usuarios.datos[indiceEncontrado]);

    tUsuario usuarioEditado = usuarios.datos[indiceEncontrado];

    char opcion;
    do {
        printf("\n---| CAMPOS A EDITAR |---\n");
        printf("[1] Email\n");
        printf("[2] Password\n");
        printf("[3] Nombre completo\n");
        printf("[4] Rol\n");
        printf("[x] Terminar edicion\n");
        printf("Seleccione una opcion: ");

        if (scanf(" %c", &opcion) != 1) {
            printf("Entrada invalida.\n");
            fflush(stdin);
            continue;
        }

        switch (opcion) {
        case '1': {
            tString nuevoEmail;
            do {
                ingresarCampo("Ingrese el nuevo email: ", nuevoEmail);
                minus(nuevoEmail);
                if (emailExiste(nuevoEmail) && strcmp(nuevoEmail, usuarioEditado.email) != 0) {
                    printf("Error: El email '%s' ya existe.\n", nuevoEmail);
                } else {
                    strcpy(usuarioEditado.email, nuevoEmail);
                    printf("Email actualizado correctamente.\n");
                    break;
                }
            } while (1);
            break;
        }

        case '2':
            ingresarCampo("Ingrese la nueva password: ", usuarioEditado.password);
            printf("Password actualizada correctamente.\n");
            break;

        case '3':
            ingresarCampo("Ingrese el nuevo nombre completo: ", usuarioEditado.fullName);
            formatearNombre(usuarioEditado.fullName);
            printf("Nombre completo actualizado correctamente.\n");
            break;

        case '4': {
            tString nuevoRol;
            seleccionarRol(nuevoRol);

            strcpy(usuarioEditado.role, nuevoRol);
            printf("Rol actualizado correctamente.\n");
            break;
        }

        case 'x':
            break;

        default:
            printf("Opcion invalida. Intente nuevamente.\n");
        }

        if (opcion != 'x') {
            printf("\nUsuario actualizado (temporal):\n");
            imprimirUsuarioDetallado(usuarioEditado);
        }

    } while (opcion != 'x');

    obtenerFechaHora(usuarioEditado.updatedAt);

    usuarios.datos[indiceEncontrado] = usuarioEditado;

    if (escribirArchivo(usuarios)) {
        printf("\nEl usuario con ID: %s fue actualizado exitosamente.\n", id);
    } else {
        printf("\nError al guardar los cambios en el archivo.\n");
    }

    free(usuarios.datos);
}

void eliminarUsuario() {
    tString id;
    ingresarCampo("Ingrese el ID del usuario a eliminar: ", id);
    mayus(id);

    tLista usuarios = leerArchivo();

    if (!usuarios.datos || usuarios.tam == 0) {
        printf("No hay usuarios para eliminar.\n");
        return;
    }

    int indiceEncontrado = -1;
    tUsuario usuarioEliminar;

    for (int i = 0; i < usuarios.tam; i++) {
        if (strcmp(usuarios.datos[i].id, id) == 0) {
            indiceEncontrado = i;
            usuarioEliminar = usuarios.datos[i];
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Error: El usuario con ID: %s no se encontro.\n", id);
        free(usuarios.datos);
        return;
    }

    tLista nuevaLista = {NULL, 0};
    nuevaLista.datos = malloc((usuarios.tam - 1) * sizeof(tUsuario));

    if (!nuevaLista.datos && usuarios.tam > 1) {
        printf("Error de memoria.\n");
        free(usuarios.datos);
        return;
    }

    for (int i = 0; i < usuarios.tam; i++) {
        if (i != indiceEncontrado) {
            nuevaLista.datos[nuevaLista.tam] = usuarios.datos[i];
            nuevaLista.tam++;
        }
    }

    if (escribirArchivo(nuevaLista)) {
        printf("El usuario '%s' con ID: %s fue eliminado con exito.\n",
               usuarioEliminar.fullName, usuarioEliminar.id);
    }

    free(usuarios.datos);
    free(nuevaLista.datos);
}