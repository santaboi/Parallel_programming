#include <stdio.h>  // printf()
#include <limits.h> // UINT_MAX
#include "mpi.h"

int checkCircuit(int, int); //each process for 2 bits

int main(int argc, char *argv[])
{
   int i;               /* loop variable (16 bits) */
   int count = 0;       /* total count */
   int local_count = 0; //local count
   int comm_sz;         // total process
   int my_rank;         //the process making this call
   int batch_size;
   int start;
   int end;

   MPI_Init(NULL, NULL); // mpi session start
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   batch_size = USHRT_MAX / comm_sz; //the number of i each proccess should handle
   start = 0 + my_rank * batch_size; //for loop start i
   end = start + batch_size + 1;     // for loop end i

   if (my_rank + 1 == comm_sz) //consider the last process (in case , remainder)
   {
      end = USHRT_MAX + 1;
   }

   if (my_rank != 0)
   {
      // printf("start = %d , end = %d", start, end);
      for (i = start; i < end; i++)
      {
         if (checkCircuit(my_rank, i))
         {
            local_count++;
         }
      }
      MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); //send local_count to count
   }
   else // my_rank == 0
   {
      double startTime = 0.0, totalTime = 0.0;
      startTime = MPI_Wtime(); //only (rank 0) should count time
      int source;
      for (source = 1; source < comm_sz; ++source)
      {
         MPI_Recv(&local_count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recv local_count for count
         count += local_count;
      }
      printf("\nA total of %d solutions were found.\n\n", count);
      totalTime = MPI_Wtime() - startTime;
      printf("Process %d finished in time %f secs.\n", my_rank, totalTime);
      fflush(stdout);
   }

   MPI_Finalize();

   return 0;
}

/* EXTRACT_BIT is a macro that extracts the ith bit of number n.
 *
 * parameters: n, a number;
 *             i, the position of the bit we want to know.
 *
 * return: 1 if 'i'th bit of 'n' is 1; 0 otherwise 
 */

#define EXTRACT_BIT(n, i) ((n & (1 << i)) ? 1 : 0)

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */

#define SIZE 16

int checkCircuit(int id, int bits)
{
   int v[SIZE]; /* Each element is a bit of bits */
   int i;

   for (i = 0; i < SIZE; i++)
      v[i] = EXTRACT_BIT(bits, i);

   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]))
   {
      printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout);
      return 1;
   }
   else
   {
      return 0;
   }
}
