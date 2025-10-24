#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAXCAMPO 100

void mayus(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void obtenerFechaHora(char* destino) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(destino, MAXCAMPO, "%Y-%m-%d %H:%M:%S", tm_info);
}

void formatearNombre(char* name) {
    int i = 0;
    int capitalizeNext = 1;

    while (name[i]) {
        if (isspace(name[i])) {
            capitalizeNext = 1;
        } else {
            if (capitalizeNext) {
                name[i] = toupper(name[i]);
                capitalizeNext = 0;
            } else {
                name[i] = tolower(name[i]);
            }
        }
        i++;
    }
}

void formatearCodigo(char* input) {
    int i = 0, j = 0;
    int length = strlen(input);

    while (isspace(input[i]))
        i++;

    for (; i < length; i++) {
        if (isspace(input[i])) {
            if (j > 0 && input[j - 1] != '_') {
                input[j++] = '_';
            }
        } else {
            input[j++] = toupper(input[i]);
        }
    }

    if (j > 0 && input[j - 1] == '_')
        j--;

    input[j] = '\0';
}
