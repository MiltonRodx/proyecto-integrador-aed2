#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCATEGORIA 30
#define MAXLINEAS 100
#define ARCHIVO "build/stock/categories/categories.txt"

void mayus(char* str) {
  for (int i = 0; str[i]; i++) {
    str[i] = toupper((unsigned char)str[i]);
  }
}

int categoriaExiste(const char* nuevaCategoria) {
  FILE* archivo = fopen(ARCHIVO, "r");
  if (!archivo) {
    return 0;
  }

  char linea[MAXCATEGORIA];
  while (fgets(linea, MAXCATEGORIA, archivo)) {
    linea[strcspn(linea, "\n")] = 0; // Quitar salto de línea
    if (strcmp(linea, nuevaCategoria) == 0) {
      fclose(archivo);
      return 1;
    }
  }

  fclose(archivo);
  return 0;
}

void crearCategoria() {
  FILE* archivo = fopen(ARCHIVO, "a");
  if (!archivo) {
    printf("Error: No se pudo abrir o crear el archivo '%s'.\n", ARCHIVO);
    return;
  }

  char nuevaCategoria[MAXCATEGORIA];

  fflush(stdin);
  printf("Ingrese el nombre de la nueva categoria: ");
  scanf("%29[^\n]", nuevaCategoria);

  if (strlen(nuevaCategoria) == 0) {
    printf("Error: La categoria no puede estar vacia.\n");
    return;
  }

  mayus(nuevaCategoria);

  if (categoriaExiste(nuevaCategoria)) {
    printf("Error: La categoria '%s' ya existe.\n", nuevaCategoria);
    return;
  }

  fprintf(archivo, "%s\n", nuevaCategoria);
  fclose(archivo);

  printf("Categoria '%s' agregada exitosamente.\n", nuevaCategoria);
}

void leerCategorias() {
  FILE* archivo = fopen(ARCHIVO, "r");
  if (!archivo) {
    printf("No se pudo abrir el archivo '%s'. Puede que no exista aun.\n", ARCHIVO);
    return;
  }

  char categorias[MAXLINEAS][MAXCATEGORIA];
  int count = 0;

  while (fgets(categorias[count], MAXCATEGORIA, archivo) && count < MAXLINEAS) {
    categorias[count][strcspn(categorias[count], "\n")] = 0; // quitar salto de línea
    count++;
  }

  fclose(archivo);

  if (count == 0) {
    printf("No hay categorias en el archivo.\n");
    return;
  }

  printf("Categorías actuales:\n");
  for (int i = 0; i < count; i++) {
    printf("%d. %s\n", i + 1, categorias[i]);
  }
}