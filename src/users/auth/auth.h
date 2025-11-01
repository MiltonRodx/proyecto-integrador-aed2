#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include "../users.h"

#define MAX_INTENTOS 3
#define MAX_LEN 64
#define MAXLINEA 300

extern bool estaLoggeado;
extern tUsuario usuarioActual;

void login();
void logout();

bool esAdmin();
bool esInventoryManager();
bool esCashier();
bool esClient();
bool tienePermiso(const char* rolRequerido);

tUsuario* getUsuarioActual();
bool isLoggedIn();

#endif