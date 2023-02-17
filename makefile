CC=gcc
CFLAGS=-I.
DEPS = Linklist.h
OBJ = Program.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pro: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
