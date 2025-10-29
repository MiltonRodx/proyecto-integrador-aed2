#ifndef AUTH_H
#define AUTH_H
#include <stdbool.h>

void obtenerPassword();
void obtenerUser();
void recorrerCSVdeLogin();
void login();
extern bool estaLoggeado;

#endif