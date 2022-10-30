CC = g++
C = gcc
file = pi.cpp
out = pi.out
CFlag = -O3 -Wall
LinkFlag = -lpthread
SimdCFlag = -fPIC -std=c99 -O3 -mavx2  -march=native -Wall -Wextra -pedantic -Wshadow
obj = simdxorshift128plus.o pi.o


all: $(obj)
	$(CC) -o $(out) $(obj) $(LinkFlag)

pi.o: $(file)
	$(CC) $(CFlag) -c $(file)

simdxorshift128plus.o: simdxorshift128plus.c
	$(C) $(SimdCFlag) -c simdxorshift128plus.c

clean:
	rm *.out *.o
