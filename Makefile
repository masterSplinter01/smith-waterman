CC=g++
CFLAGS = -Wall -Werror

all: sw

sw: main.o smith_waterman.o
	$(CC) $(CFLAGS) main.o smith_waterman.o -o sw
main.o: ./src/main.cpp
	$(CC) $(CFLAGS) -c ./src/main.cpp

smith_waterman.o: ./src/smith_waterman.cpp ./src/smith_waterman.h
	$(CC) $(CFLAGS) -c ./src/smith_waterman.cpp

clean:
	rm -rf *.o bot

valgrind:
	 valgrind --leak-check=yes --show-reachable=yes ./sw

