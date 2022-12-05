all: mpi_hello copy

mpi_hello: hello.cc
	mpicxx hello.cc -o mpi_hello

copy:
	parallel-scp -h host.txt -r ~/HW4 ~

run:
	mpirun -np 16 --hostfile host.txt mpi_hello

clean:
	rm -f mpi_hello
