#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

long compute_number_in_circle(long number_of_tosses, unsigned int seed){
    // __builtin_assume(N == 1024);
    long number_in_circle = 0;
    double dist;
    double x, y;
    for(long i = 0; i < number_of_tosses; i++){
        x = (double) rand_r(&seed) / (RAND_MAX), 2;
        y = (double) rand_r(&seed) / (RAND_MAX), 2;
        dist = x * x + y * y;
        if(dist <= 1)
            number_in_circle++;
    }

    return number_in_circle;
}

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: init MPI
    int source;
    int dest;
    int tag = 0;
    MPI_Status status;
    MPI_Comm_size( MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank( MPI_COMM_WORLD, &world_rank);
    long long int number_of_tosses = tosses / world_size;
    long long int number_in_circle = compute_number_in_circle(number_of_tosses, (unsigned int)world_rank);
    long long int total = 0;

    if (world_rank > 0)
    {
        // TODO: handle workers
        dest = 0;
        MPI_Request req;
        MPI_Isend(&number_in_circle, 1, MPI_LONG, dest, tag, MPI_COMM_WORLD, &req);
    }
    else if (world_rank == 0)
    {
        // TODO: master
        // compute its own number of tosses in master node
        
        // sum of number of tosses
        long long int buf[world_size];
        buf[0] = number_in_circle;
        int num_request = world_size - 1;
	    MPI_Status status[num_request];
        MPI_Request requests[num_request];
        for(source = 1; source < world_size; source++){
            MPI_Irecv(&buf[source], 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &requests[source-1]);
        }
        MPI_Waitall(num_request, requests, status);
        for (int i = 0; i < world_size; i++){
            total += buf[i];
        }
    }

    if (world_rank == 0)
    {
        // TODO: process PI result
        pi_result = 4 * total / (double) tosses;
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
