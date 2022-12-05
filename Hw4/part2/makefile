CXX=mpicxx
SRC=main.cc
SRC+= matrix.cc

EXE=matmul

OBJ=${SRC:.cc=.o}

all: ${EXE}

${EXE}: ${OBJ}
	${CXX} -o $@ $^

%.o: %.cc
	${CXX} -c $^ -o $@

run:
	parallel-scp -h host.txt -r ~/HW4 ~
	mpirun -np 4 --hostfile /home/.grade/HW4/mat2_hosts ./matmul < test.txt

.PHONY: clean

clean:
	rm -rf ${OBJ} ${EXE}
