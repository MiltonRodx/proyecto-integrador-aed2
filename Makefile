TARGET = program
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -I$(INC_DIR)

# Buscar todos los archivos .c (incluye subcarpetas)
SRCS = $(shell find $(SRC_DIR) -name "*.c")

# Generar lista de archivos .o correspondientes
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Regla principal
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET)

# Compilar cada .c en su .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar
clean:
	rm -rf $(BUILD_DIR)

# Ejecutar el programa
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

.PHONY: clean run