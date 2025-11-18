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

# Buscar archivos de datos (CSV y TXT)
DATA_FILES = $(shell find $(SRC_DIR) -name "*.csv" -o -name "*.txt")
DATA_TARGETS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(DATA_FILES))

# Regla principal - ahora incluye los archivos de datos
$(BUILD_DIR)/$(TARGET): $(OBJS) $(DATA_TARGETS)
	$(CC) $(OBJS) -o $(BUILD_DIR)/$(TARGET)

# Compilar cada .c en su .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Copiar archivos de datos (CSV y TXT)
$(BUILD_DIR)/%: $(SRC_DIR)/%
	mkdir -p $(dir $@)
	cp $< $@

# Limpiar
clean:
	rm -rf $(BUILD_DIR)

# Ejecutar el programa
run: $(BUILD_DIR)/$(TARGET)
	./$(BUILD_DIR)/$(TARGET)

.PHONY: clean run