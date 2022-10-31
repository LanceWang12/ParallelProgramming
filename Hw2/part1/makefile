CC = clang++
C = gcc
file = pi.cpp
out = pi.out
CFlag = -std=c++11 -O3 -Wall #-Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize
LinkFlag = -lpthread
SimdCFlag = -fPIC -std=c99 -O3 -mavx2  -march=native -Wall -Wextra -pedantic -Wshadow
obj = pi.o


all: $(obj)
	$(CC) -o $(out) $(obj) $(LinkFlag)

pi.o: $(file)
	$(CC) $(CFlag) -c $(file)

# simdxorshift128plus.o: simdxorshift128plus.c
# 	$(C) $(SimdCFlag) -c simdxorshift128plus.c

clean:
	rm *.out *.o
