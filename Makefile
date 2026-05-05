CC = gcc
CFLAGS = -O2 -Wall `pkg-config --cflags gtk+-3.0`
LDFLAGS = -lm `pkg-config --libs gtk+-3.0`

SRC = src/main.c src/parser.c src/eval.c src/ui.c
OBJ = $(SRC:.c=.o)
TARGET = calc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean