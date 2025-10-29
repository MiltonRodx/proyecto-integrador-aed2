#include "users.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "functions.h"

#define MAX_LEN 64
#define MAX_USERS 200
typedef char tString[MAX_LEN];
#define MAX 64
typedef char tString[MAX];
tString roles[4] = {"admin", "inventory_manager", "manager", "client"};
#define MAX_LINE 512


//Estructura tUsuario
typedef struct {
    int id;
    char password[MAX_LEN];
    char email[MAX_LEN];
    char firstName[MAX_LEN];
    char lastName[MAX_LEN];
    int rol;
    char createdAt[MAX_LEN];
    char updatedAt[MAX_LEN];
} tUsuario;

tUsuario usuarios[MAX_USERS];
int numUsuarios = 0;


//                  //
//    Definicion    //
//                  //
//Create
void crearUsuario(){
    FILE* file = fopen("users.csv", "a");

    //Verif
    if (!file) {
        perror("Error al abrir users.csv");
        return;
    }

    int id;
    char username[MAX];
    char password[MAX];
    char email[MAX];
    char nombres[MAX];
    char apellidos[MAX];
    char rol[MAX];
    char horaCreado[32];
    char horaActualizado[32];

    //Desplazamiento correcto hacia caracter deseado
    // Que seria el ultimo \n
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size > 0) {
        int ch;

        while (ch == '\n' || ch == '\r'){

            fseek(file, -1, SEEK_CUR);
            ch = fgetc(file);
            if (ftell(file) <= 1) break; //para si esta en inicio
        }
      
        if (ch != '\n' && ch != '\r') {
            fputc('\n', file);
        }
    }


    // obtener informacion de fecha y hora
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(horaCreado, sizeof(horaCreado), "%Y-%m-%d", tm_info);
    strncpy(horaActualizado, horaCreado, sizeof(horaActualizado));

    // Input de datos
    printf("Ingrese ID: ");
    scanf("%d", &id);
    printf("Ingrese username: ");
    scanf("%s", username);
    printf("Ingrese password: ");
    scanf("%s", password);
    printf("Ingrese email: ");
    scanf(" %s", email);
    printf("Ingrese nombres: ");
    scanf(" %63[^\n]", nombres);
    printf("Ingrese apellido: ");
    scanf(" %63[^\n]", apellidos);
    printf("Ingrese rol (admin, inventory_manager, manager, client): ");
    scanf("%s", rol);

    // Guardar en el CSV
    fprintf(file, "%d,%s,%s,%s,%s,%s,%s,%s,%s",
            id, username, password, email, nombres, apellidos, rol, horaCreado, horaActualizado);

    fclose(file);
    printf("Usuario creado y guardado en users.csv\n");
}





void buscarUserPorID(int id) {
    FILE* file = fopen("users.csv", "r");
    if (!file) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // saltar encabezado

    while (fgets(line, sizeof(line), file)) {
        int userId;
        if (sscanf(line, "%d,", &userId) == 1 && userId == id) {
            printf("Usuario encontrado:\n%s", line);
            fclose(file);
            return;
        }
    }

    printf("Usuario con ID %d no encontrado.\n", id);
    fclose(file);
}

void buscarPorUsername(tString nombre) {
    FILE* file = fopen("users.csv", "r");
    if (!file) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // saltar encabezado

    while (fgets(line, sizeof(line), file)) {
        char userId[10], username[50];
        // Extraemos id y username (primeros 2 campos)
        if (sscanf(line, "%[^,],%[^,]", userId, username) == 2) {
            if (strcmp(username, nombre) == 0) {
                printf("Usuario encontrado:\n%s", line);
                fclose(file);
                return;
            }
        }
    }

    printf("Usuario con username '%s' no encontrado.\n", nombre);
    fclose(file);
}


void buscarPorEmail(tString email) {
    FILE* file = fopen("users.csv", "r");
    if (!file) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char line[512];
    fgets(line, sizeof(line), file); // saltar encabezado

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, email)) { // si la línea contiene el email
            printf("Usuario encontrado:\n%s", line);
            fclose(file);
            return;
        }
    }

    printf("Usuario con email '%s' no encontrado.\n", email);
    fclose(file);
}



void mostrarUsuarios() {
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        perror("Error al abrir users.csv");
        return;
    }

    bool esCabecera = true;
    char line[MAX_LINE];

    printf("=== LISTA DE USUARIOS ===\n\n");
    printf("%-4s %-12s %-12s %-25s %-12s %-12s %-8s %-12s %-12s\n",
           "ID", "User", "Password", "Email", "Nombre", "Apellido", "Rol", "Creado", "Actualizado");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // remove newline

        if (strlen(line) == 0) continue;

        // modificar cabecera
        if (esCabecera) {
            esCabecera = false;
            continue;
        }

        // usar un token, la coma
        char *token = strtok(line, ",");
        char *campos[9];
        int i = 0;

        while (token != NULL && i < 9) {
            campos[i++] = token;
            token = strtok(NULL, ",");
        }

        if (i == 9) {
            printf("%-4s %-12s %-12s %-25s %-12s %-12s %-8s %-12s %-12s\n",
                   campos[0], campos[1], campos[2], campos[3],
                   campos[4], campos[5], campos[6], campos[7], campos[8]);
        }
    }
    printf("\n");
    fclose(file);
}



void listarPorRol(int indiceRol) {
    if (indiceRol < 0 || indiceRol > 3) {
        printf("Rol inválido.\n");
        return;
    }

    const char *rolBuscado = roles[indiceRol];

    FILE* archivo = fopen("users.csv", "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char linea[512];
    fgets(linea, sizeof(linea), archivo); // saltar encabezado

    printf("Usuarios con rol '%s':\n", rolBuscado);
    printf("%-4s %-12s %-25s %-12s %-12s %-12s %-12s\n",
           "ID", "Usuario", "Email", "Nombre", "Apellido", "Creado", "Actualizado");
    printf("--------------------------------------------------------------------------------\n");

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0'; // eliminar salto de línea

        char *token;
        char campos[9][128]; // 9 campos por línea
        int i = 0;

        token = strtok(linea, ",");
        while (token != NULL && i < 9) {
            strncpy(campos[i], token, sizeof(campos[i]));
            i++;
            token = strtok(NULL, ",");
        }

        // Comprobar que el rol coincide
        if (i == 9 && strcmp(campos[6], rolBuscado) == 0) {
            printf("%-4s %-12s %-25s %-12s %-12s %-12s %-12s\n",
                   campos[0], campos[1], campos[3], campos[4], campos[5], campos[7], campos[8]);
        }
    }

    fclose(archivo);
}

void listarUsuarios(){
    //menu
    char opc;

    do {
        printf("\n=== Menu: Listar usuarios ===\n");
        printf("1. Listar todos\n");
        printf("2. Listar por rol\n");
        printf("x. Opcion anterior\n");
        scanf(" %c", &opc);

        switch (opc){
            case '1': mostrarUsuarios();
                    break;
            case '2': int pInt_Rol;
                    printf("Inserte numero de rol: (1= ADMIN, 2= INVENTORY_MANAGER, 3= CASHIER, 4= CLIENT)");
                    scanf(" %d", &pInt_Rol);
                    listarPorRol(pInt_Rol-1);
                    break;
            default: printf("Opción inválida\n");
                    break;
        }
    } while (opc != 'x');

    
}

void buscarUsuarios(){
    char opc;
    do {
        printf("\n=== Menu: Buscar usuarios ===\n");
        printf("1. Buscar por ID\n");
        printf("2. Buscar por user\n");
        printf("3. Buscar por email\n");
        printf("x. Opcion anterior\n");
        scanf(" %c", &opc);

        switch (opc){
            case '1': int pId;
                printf("Inserte id: ");
                scanf(" %d", &pId);
                buscarUserPorID(pId);
                break;
            case '2': tString pUsuario;
                printf("Inserte usuario: ");
                scanf(" %63[^\n]", pUsuario);
                buscarPorUsername(pUsuario);
                break;
            case '3': tString pEmail;
                printf("Inserte email: ");
                scanf(" %63[^\n]", pEmail);
                buscarPorEmail(pEmail);
                break;
            default: printf("Opcion invalida\n");
                break;
        }
    } while (opc != 'x');
}


//Read
void menuLectura(){
    char opc;
    do {
        printf("\n=== Menu: Lectura de archivo ===\n");
        printf("1. Listar\n");
        printf("2. Buscar\n");
        printf("x. Opcion anterior\n");
        scanf(" %d", &opc);

        switch (opc) {
            case '1': listarUsuarios();
                    break;
            case '2': buscarUsuarios();
                    break;
            default: printf("Opcion invalida\n");
                    break;
        }
    } while (opc != 'x');
}


//Update
void actualizarUsername(int id, tString nuevoUsername) {
    FILE* file = fopen("users.csv", "r");
    FILE* temp = fopen("temp.csv", "w");
    char line[512];

    if (!file || !temp) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    // Copiar encabezado
    if (fgets(line, sizeof(line), file)) {
        fputs(line, temp);
    }

    bool encontrado = false;

    while (fgets(line, sizeof(line), file)) {
        int userId;
        char username[50], rest[400];

        // Separar ID y username, el resto se deja tal cual.
        if (sscanf(line, "%d,%49[^,],%[^\n]", &userId, username, rest) == 3) {
            if (userId == id) {
                fprintf(temp, "%d,%s,%s\n", userId, nuevoUsername, rest);
                encontrado = true;
            } else {
                fputs(line, temp);
            }
        }
    }

    fclose(file);
    fclose(temp);

    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (encontrado) {
        printf("Username del usuario %d actualizado a '%s'.\n", id, nuevoUsername);
    } else {
        printf("Usuario con ID %d no encontrado.\n", id);
    }
}

void actualizarEmail(int id, tString nuevoEmail) {
    FILE* file = fopen("users.csv", "r");
    FILE* temp = fopen("temp.csv", "w");
    char line[512];

    if (!file || !temp) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    // Copiar encabezado
    if (fgets(line, sizeof(line), file)) {
        fputs(line, temp);
    }

    int encontrado = 0;

    while (fgets(line, sizeof(line), file)) {
        int userId;
        char username[50], password[50], email[100], resto[300];

        // Separar los primeros 4 campos, el resto se mantiene igual
        if (sscanf(line, "%d,%49[^,],%49[^,],%99[^,],%[^\n]", &userId, username, password, email, resto) == 5) {
            if (userId == id) {
                // Escribir línea con el nuevo email
                fprintf(temp, "%d,%s,%s,%s,%s\n", userId, username, password, nuevoEmail, resto);
                encontrado = 1;
            } else {
                fputs(line, temp);
            }
        }
    }

    fclose(file);
    fclose(temp);

    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (encontrado) {
        printf("Email del usuario %d actualizado a '%s'.\n", id, nuevoEmail);
    } else {
        printf("Usuario con ID %d no encontrado.\n", id);
    }
}

void actualizarContrasena(int id, tString nuevaContrasena) {
    FILE* file = fopen("users.csv", "r");
    FILE* temp = fopen("temp.csv", "w");
    char line[512];

    if (!file || !temp) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    // Copiar encabezado
    if (fgets(line, sizeof(line), file)) {
        fputs(line, temp);
    }

    int encontrado = 0;

    while (fgets(line, sizeof(line), file)) {
        int userId;
        char username[50], password[50], email[100], rest[300];

        // Separar los primeros 4 campos y el resto
        if (sscanf(line, "%d,%49[^,],%49[^,],%99[^,],%[^\n]", &userId, username, password, email, rest) == 5) {
            if (userId == id) {
                // Escribir línea con nueva contraseña
                fprintf(temp, "%d,%s,%s,%s,%s\n", userId, username, nuevaContrasena, email, rest);
                encontrado = 1;
            } else {
                fputs(line, temp);
            }
        }
    }

    fclose(file);
    fclose(temp);

    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (encontrado) {
        printf("Contraseña del usuario %d actualizada.\n", id);
    } else {
        printf("Usuario con ID %d no encontrado.\n", id);
    }
}


//Delete
void borrarUsuario(int id){
    FILE *file = fopen("users.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    char line[512];

    if (!file || !temp) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    // Copiar el encabezado
    if (fgets(line, sizeof(line), file)) {
        fputs(line, temp);
    }

    int encontrado = 0;

        // Leer cada línea y copiar solo si no coincide el ID
    while (fgets(line, sizeof(line), file)) {
        int userId;
        if (sscanf(line, "%d,", &userId) == 1) {
            if (userId != id) {
                fputs(line, temp);
            } else {
                encontrado = 1;
            }
        }
    }

    fclose(file);
    fclose(temp);

    // Reemplazar el archivo original
    remove("users.csv");
    rename("temp.csv", "users.csv");

    if (encontrado) {
        printf("Usuario con ID %d eliminado correctamente.\n", id);
    } else {
        printf("Usuario con ID %d no encontrado.\n", id);
    }
}

void actualizarUsuario(){
    //menu
    char opc;
    printf("=== Menu: Actualizar usuario ===\n");
    printf("1. Actualizar username\n");
    printf("2. Actualizar email\n");
    printf("3. Actualizar contrasena\n");
    printf("x. Opcion anterior\n");
    scanf(" %c", &opc);
    
    do {
        switch (opc) {
            case '1': int pId; tString new_usern;
                    printf("Inserte nuevo username: ");
                    scanf(" %s", &new_usern);
                    printf("Inserte id: ");
                    scanf("%d", &pId);
                    actualizarUsername(pId, new_usern);
                    break;
            case '2': int pId1; tString new_email;
                    printf("Inserte nuevo email: ");
                    scanf(" %s", &new_email);
                    printf("Inserte id: ");
                    scanf("%d", &pId1);
                    actualizarEmail(pId1, new_email);
                    break;
            case '3': int pId2; tString new_passwd;
                    printf("Inserte nueva contrasena: ");
                    scanf(" %s", &new_passwd);
                    printf("Inserte id: ");
                    scanf("%d", &pId2);
                    actualizarContrasena(pId2, new_passwd);
                    break;
            default: printf("Opcion Invalida\n");
                    break;
        }
    } while (opc != 'x');
}