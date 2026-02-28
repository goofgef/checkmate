CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = checkmate

all:
	$(CC) main.c $(CFLAGS) -o $(TARGET)

clean:
	del checkmate.exe