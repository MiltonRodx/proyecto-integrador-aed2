#ifndef USERS_H
#define USERS_H

#include <stdbool.h>
#include "../types.h"

#define MAXPASS 30
#define MAX_INTENTOS 3
#define MAXLINEA 300
#define MAX_USERS 200
#define MAX_LINE 512
#define MAXCAMPO 50
#define ARCHIVO_USUARIOS "build/users/users.csv"
#define CANT_ROLE 4

typedef struct {
    tId id;
    tString email;
    tString password;
    tString fullName;
    tString role;
    tString createdAt;
    tString updatedAt;
} tUsuario;

typedef struct {
    tUsuario* datos;
    int tam;
} tListaUsuarios;

extern tString roles[CANT_ROLE];

extern bool estaLoggeado;
extern tUsuario usuarioActual;

void login();
void logout();

bool isLoggedIn();
tUsuario* getUsuarioActual();

bool esAdmin();
bool esInventoryManager();
bool esCashier();
bool esClient();
bool tienePermiso(const char* rolRequerido);

void parsearUsuario(char*, tUsuario*);
void crearUsuario();
void listarUsuarios();
void menuBusquedaUsuarios();
void actualizarUsuario();
void eliminarUsuario();

void menuUsuarios();

#endif