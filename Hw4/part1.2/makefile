CC=mpicxx
RUN=mpirun
HOSTFILE=host.txt
TOSSES=1000000000

all:
	@echo Please add the tag

# compile and run linear
linear: pi_block_linear copy
	$(RUN) -np 12 --hostfile $(HOSTFILE) pi_block_linear $(TOSSES)
pi_block_linear: pi_block_linear.cc
	$(CC) pi_block_linear.cc -o pi_block_linear

# compile and run tree
tree: pi_block_tree copy
	$(RUN) -np 16 --hostfile $(HOSTFILE) pi_block_tree $(TOSSES)
pi_block_tree: pi_block_tree.cc
	$(CC) pi_block_tree.cc -o pi_block_tree

# compile and run non-block linear
nonblock: pi_nonblock_linear copy
	$(RUN) -np 16 --hostfile $(HOSTFILE) pi_nonblock_linear $(TOSSES)
pi_nonblock_linear: pi_nonblock_linear.cc
	$(CC) pi_nonblock_linear.cc -o pi_nonblock_linear

# compile and run gather
gather: pi_gather copy
	$(RUN) -np 12 --hostfile $(HOSTFILE) pi_gather $(TOSSES)
pi_gather: pi_gather.cc
	$(CC) pi_gather.cc -o pi_gather

# compile and run gather
reduce: pi_reduce copy
	$(RUN) -np 16 --hostfile $(HOSTFILE) pi_reduce $(TOSSES)
pi_reduce: pi_reduce.cc
	$(CC) pi_reduce.cc -o pi_reduce

copy:
	parallel-scp -h host.txt -r ~/HW4 ~

clean:
	rm -f pi_block_linear pi_block_tree pi_nonblock_linear