CC = clang
CFLAGS = -O3 -Iinclude

rubicon.o: src/rubicon.c include/rubicon.h
	$(CC) $(CFLAGS) -c src/rubicon.c -o rubicon.o

clean:
	rm -f rubicon.o