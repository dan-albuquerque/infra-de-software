EXECUTABLE = banker

CC = gcc

CFLAGS = -Wall -Wextra

SRC_DIR = .

# Obtenha todos os arquivos-fonte no diretório SRC_DIR
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Obtenha todos os arquivos-objeto
OBJECTS = $(SOURCES:.c=.o)

# Regra padrão para construir o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

# Regra para construir os arquivos-objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove os arquivos-objeto e o executável
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)