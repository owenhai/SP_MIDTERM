CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
OBJ = ls.o print.o cmp.o
TARGET = ls

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
