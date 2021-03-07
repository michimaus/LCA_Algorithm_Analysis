CC = g++
CFLAGS = -Wall -Wextra -O2
LDLIBS = -lm

build: algo1.o algo2.o algo3.o

algo1.o: algo1.cpp
	g++ algo1.cpp -c $(CFLAGS)

algo2.o: algo2.cpp
	g++ algo2.cpp -c $(CFLAGS)

algo3.o: algo3.cpp
	g++ algo3.cpp -c $(CFLAGS)

.PHONY: clean
clean:
	rm -rf *.o
