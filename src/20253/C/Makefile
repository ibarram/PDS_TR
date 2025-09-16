CC = gcc
001 = 01_Ejemplo
LIB = libarram

all: $(001)

$(LIB).o: $(LIB).c
	$(CC) -c $(LIB).c -o $(LIB).o

lib$(LIB).a: $(LIB).o
	ar rcs lib$(LIB).a $(LIB).o

$(001):  $(001).c lib$(LIB).a
	$(CC) $(001).c -L. -l$(LIB) -o $(001)

clean:
	rm -f $(001) $(LIB).o lib$(LIB).a