#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define SEED 121313

int main(int argc, char *argv[])
{
    long n = 6500;
    int myid;         //proccess rank id
    double x, y;      //x,y value for the random coordinate
    double z;         //whether if x^2+y^2 <=1
    int i, count = 0; //Count holds all the number of how many good coordinates
    double pi;
    int node;

    MPI_Init(&argc, &argv); //Start MPI
    double startTime = 0.0, totalTime = 0.0;
    startTime = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &myid); //get rank of node's process
    MPI_Comm_size(MPI_COMM_WORLD, &node);
    int rec1[node];
    long rec2[node];    //array for n
    srand(SEED + myid); //Give rand() a seed value. Needs to be different on each node

    if (myid != 0)
    {
        for (i = 0; i < n; ++i) //main loop
        {
            x = ((double)rand()) / RAND_MAX; //gets a random x coordinate
            y = ((double)rand()) / RAND_MAX; //gets a random y coordinate
            z = sqrt(x * x + y * y);         //Checks to see if number in inside unit circle
            if (z <= 1)
            {
                count++; //if it is, consider it a valid random point
            }
        }
        for (i = 0; i < node; ++i)
        {
            MPI_Send(&count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD); //tag1
            MPI_Send(&n, 1, MPI_LONG, 0, 2, MPI_COMM_WORLD);    //tag2
        }
    }
    else if (myid == 0)
    {
        for (i = 0; i < node; ++i)
        {
            MPI_Recv(&rec1[i], node, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  //tag1
            MPI_Recv(&rec2[i], node, MPI_LONG, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //tag2
        }
    }

    if (myid == 0) //if root process
    {
        long finaln = 0;
        int finalcount = 0;
        for (i = 0; i < node; ++i)
        {
            finaln += rec2[i];
            finalcount += rec1[i];
        }

        pi = 4 * ((double)finalcount / (double)finaln); //pi estimate
        printf("Pi: %f\n", pi);                         //Print pi estimated
        totalTime = MPI_Wtime() - startTime;
        printf("Process %d finished in time %f secs.\n", myid, totalTime);
    }

    MPI_Finalize(); //MPI close
    return 0;
}