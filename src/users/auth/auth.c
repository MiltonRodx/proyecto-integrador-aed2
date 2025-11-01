// Librerias
#include "auth.h"
#include "../functions.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Variables globales
char email[MAX_LEN];
char password[MAX_LEN];
int contIntentos = 0;
bool estaLoggeado = false;
tUsuario usuarioActual; // Usuario que está loggeado

void obtenerPassword() {
#ifdef _WIN32
  int i = 0;
  int c;
  printf("Contrasena: ");
  while ((c = _getch()) != '\r' && i < MAX_LEN - 1) {
    if (c == '\b' || c == 63) {
      if (i > 0) {
        i--;
        printf("\b \b");
      }
    } else {
      password[i++] = (char)c;
      putchar('*');
    }
  }
  password[i] = '\0';
  putchar('\n');
  fflush(stdin);
#else
  char* pw = getpass("Contrasena: ");
  if (pw) {
    strncpy(password, pw, MAX_LEN - 1);
    password[MAX_LEN - 1] = '\0';
  } else {
    password[0] = '\0';
  }
#endif
}

void obtenerEmail() {
  printf("Email: ");
  scanf("%s", email);
}

bool autenticarUsuario(const char* emailInput, const char* passwordInput, tUsuario* usuario) {
  FILE* file = fopen(ARCHIVO, "r");

  if (!file) {
    printf("Error: No se pudo abrir el archivo de usuarios.\n");
    return false;
  }

  char linea[MAXLINEA];
  int esPrimeraLinea = 1;

  while (fgets(linea, MAXLINEA, file)) {
    linea[strcspn(linea, "\n")] = '\0';

    if (esPrimeraLinea) {
      esPrimeraLinea = 0;
      if (strstr(linea, "id,") != NULL) {
        continue;
      }
    }

    char lineaCopia[MAXLINEA];
    strcpy(lineaCopia, linea);

    tUsuario usuarioTemp;
    parsearUsuario(lineaCopia, &usuarioTemp);

    if (strcmp(usuarioTemp.email, emailInput) == 0 &&
        strcmp(usuarioTemp.password, passwordInput) == 0) {
      *usuario = usuarioTemp;
      fclose(file);
      return true;
    }
  }

  fclose(file);
  return false;
}

void login() {
  printf("\n=== INICIO DE SESION ===\n");

  while (contIntentos < MAX_INTENTOS) {
    obtenerEmail();
    obtenerPassword();

    if (autenticarUsuario(email, password, &usuarioActual)) {
      printf("\nInicio de sesion exitoso!\n");
      printf("\nBienvenido, %s (%s)\n", usuarioActual.fullName, usuarioActual.role);

      estaLoggeado = true;
      contIntentos = 0;
      return;
    } else {
      contIntentos++;
      printf("\nEmail o contrasena incorrecto. Intento %d de %d.\n\n",
             contIntentos, MAX_INTENTOS);
    }
  }

  if (contIntentos >= MAX_INTENTOS) {
    printf("\nDemasiados intentos fallidos. Acceso bloqueado.\n");
    exit(1);
  }
}

void logout() {
  estaLoggeado = false;
  memset(&usuarioActual, 0, sizeof(tUsuario));
  printf("\nSesion cerrada exitosamente.\n");
}

bool esAdmin() {
  return estaLoggeado && strcmp(usuarioActual.role, "ADMIN") == 0;
}

bool esInventoryManager() {
  return estaLoggeado && strcmp(usuarioActual.role, "INVENTORY_MANAGER") == 0;
}

bool esCashier() {
  return estaLoggeado && strcmp(usuarioActual.role, "CASHIER") == 0;
}

bool esClient() {
  return estaLoggeado && strcmp(usuarioActual.role, "CLIENT") == 0;
}

bool tienePermiso(const char* rolRequerido) {
  return estaLoggeado && strcmp(usuarioActual.role, rolRequerido) == 0;
}

// Obtener información del usuario logueado
tUsuario* getUsuarioActual() {
  if (estaLoggeado) {
    return &usuarioActual;
  }
  return NULL;
}

bool isLoggedIn() {
  return estaLoggeado;
}