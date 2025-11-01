#ifndef FUNCTIONS_USERS_H
#define FUNCTIONS_USERS_H

#include "users.h"

/// Constantes
#define MAX_USERS 200
#define MAX 64
#define MAX_LINE 512
#define MAXCAMPO 50
#define MAXLINEA 300
#define ARCHIVO "build/users/users.csv"
#define CANT_ROLE 4

typedef struct {
    tUsuario* datos;
    int tam;
} tLista;

// Variables globales
extern tString roles[CANT_ROLE];  

void parsearUsuario(char*, tUsuario*);
void crearUsuario();
void listarUsuarios();
void menuBusquedaUsuarios();
void actualizarUsuario();
void eliminarUsuario();

#endif
