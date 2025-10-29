#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include "auth.h"

#ifdef _WIN32 //para compatibilidad con sistemas win + unix
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

#define MAX_INTENTOS 3
#define MAX_LEN 64

char username[MAX_LEN];
char password[MAX_LEN];

const char correctUsername[] = "admin";
const char correctPassword[] = "12345678";
int contIntentos = 0;
bool estaLoggeado;

//Definición
void obtenerPassword(){
#ifdef _WIN32
    int i = 0;
    int c;
    printf("Contrasena: "); 
    /* manejo de caracteres con fin de poner 
    asteriscos en vez de mostrar contrasena */
    while ((c = _getch()) != '\r' && i < MAX_LEN - 1) { //scanear contraseña verificando
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
    password[idx] = '\0';
    putchar('\n');
    fflush(stdin);
#else
    /* permite usar getpass() para ocultar input */
    char *pw = getpass("Contrasena: ");
    if (pw) {
        // copiar la contrasena al buffer
        strncpy(password, pw, MAX_LEN-1);
        password[MAX_LEN-1] = '\0';
    } else {
        password[0] = '\0';
    }
    //nueva linea para ser consistente
#endif
}


void obtenerUser(){
    printf("Usuario: ");
    scanf("%s", username);
}

void recorrerCSVdeLogin(){
    FILE *file = fopen("users.csv", "r");
    
    //si no logra abrir archivo
    if (!file) {
        perror("Error al guardar el archivo");
        return;
    }
    //revisar
    //fprintf(file, );
}

void login(){
    printf("===Inicio de Sesión ===\n");
    while (contIntentos < MAX_INTENTOS){
        obtenerUser();
        obtenerPassword();

        // Revisar credenciales
        if (strcmp(username, correctUsername) == 0 && strcmp(password, correctPassword) == 0) {
            printf("Inicio de sesion exitoso. Bienvenido, %s.\n", username);
            estaLoggeado = true;
            break;
        } else {
            contIntentos++;
            printf("Usuario o contrasena incorrecto. Intento %d de %d.\n", contIntentos, MAX_INTENTOS);
        }
    }
}

