CC = gcc

CFLAGS = -Wall -Wextra -std=c11
CFLAGSSPECIAL = -Wall -Wextra -std=c11 -Os -s -g0
CFLAGSLIB = -Wall -Wextra -std=c11 -c

TARGET = checkmate
TARGETLIB = checkmateobj.o
TARGETA = libcheckmate.a


all:
	$(CC) main.c $(CFLAGS) -o $(TARGET)

special:
	$(CC) main.c $(CFLAGSSPECIAL) -o $(TARGET)

slib:
	$(CC) $(CFLAGSLIB) main.c -o $(TARGETLIB)
	ar rcs $(TARGETA) $(TARGETLIB)

linuxdlib:
	gcc -Wall -Wextra -std=c11 -fPIC -shared main.c -o libcheckmate.so

windlib:
	gcc -Wall -Wextra -std=c11 -shared -o checkmate.dll main.c -Wl,--out-implib,$(TARGETA)
	
clean:
	del checkmate.exe
