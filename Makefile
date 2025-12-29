CC=gcc
CFLAGS=-Wall -Wextra -std=c89 -pedantic -ansi
LIBS=-lm

TARGET=calc.exe
OBJ=main.o bigint.o stack.o evaluator.o bigint_io.o

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJ)