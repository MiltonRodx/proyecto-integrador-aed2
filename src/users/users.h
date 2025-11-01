#ifndef USERS_H
#define USERS_H

// Constantes
#define MAX_LEN 64

// Tipos
typedef char tString[MAX_LEN];
typedef char tId[5];

// Estructura usuario
typedef struct {
    tId id;
    tString email;
    tString password;
    tString fullName;
    tString role;
    tString createdAt;
    tString updatedAt;
} tUsuario;

void menuUsuarios();

#endif