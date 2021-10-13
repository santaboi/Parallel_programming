#include <stdio.h>  // printf()
#include <limits.h> // UINT_MAX
#include <mpi.h>

int checkCircuit(int, int); //each process for 2 bits

int main(int argc, char *argv[])
{
   int i;               /* loop variable (32 bits) */
   int id = 0;          /* process id */
   int count = 0;       /* total count */
   int local_count = 0; //local count
   int comm_sz = 8;     // total 8 process in the communicator
   int my_rank;         //the process making this call

   MPI_Init(NULL, NULL);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   double startTime = 0.0, totalTime = 0.0;
   startTime = MPI_Wtime();

   if (my_rank != 0)
   {
      local_count += checkCircuit(my_rank, my_rank * 2);
      MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }
   else
   {
      count = local_count;
      for (source = 1; source < comm_sz; source++)
      {
         MPI_Recv(&local_count, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         count += local_count;
      }
      printf("Process %d finished.\n", id);
      fflush(stdout);
   }

   totalTime = MPI_Wtime() - startTime;
   MPI_Finalize();

   printf("\nA total of %d solutions were found.\n\n", count);
   /*
   for (i = 0; i <= USHRT_MAX; i++)
   {
      count += checkCircuit (id, i);
   }
   */

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
   int process_count = 0;

   for (i = 0; i < SIZE; i++)
      v[i] = EXTRACT_BIT(bits, i);

   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]))
   {
      printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout);
      process_count++;
   }

   for (i = 0; i < SIZE; i++)
      v[i] = EXTRACT_BIT(bits + 1, i);

   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]))
   {
      printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout);
      process_count++;
   }

   return process_count
}
