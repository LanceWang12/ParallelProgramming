#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

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

    // TODO: MPI init
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Status status;
    long long int number_of_tosses = tosses / world_size;
    int tag = 0;
    long long int count = 0;
    long long local_cnt = compute_number_in_circle(number_of_tosses, (unsigned int)world_rank);

    // TODO: binary tree redunction
    int base = 1;
    while (base < world_size){
        MPI_Barrier(MPI_COMM_WORLD);
        if (base == 1){
            if (world_rank % 2 == 0){
            int source = world_rank + 1;
                long long int buf;
                MPI_Recv(&buf, 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
                local_cnt += buf;
            }
            else{
                int dest = world_rank - 1;
                MPI_Send(&local_cnt, 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);
            }
        }
        else{
            int mod = world_rank % base;
            int div = world_rank / base;

            if (mod == 0 && div % 2 == 0){
                int source = world_rank + base;
                long long int buf;
                MPI_Recv(&buf, 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
                local_cnt += buf;
            }
            if (mod == 0 && div % 2 != 0){
                int dest = world_rank - base;
                MPI_Send(&local_cnt, 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        base *= 2;
    }

    if (world_rank == 0)
    {
        // TODO: PI result
        count = local_cnt;
        pi_result = 4 * count / (double) tosses;

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}