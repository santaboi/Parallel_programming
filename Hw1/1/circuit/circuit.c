#include <stdio.h>  // printf()
#include <limits.h> // UINT_MAX
//#include <mpi.h>

void checkCircuit(int, int, int *);

int main(int argc, char *argv[])
{
   int my_rank, comm_sz;
   int i;         /* loop variable (32 bits) */
   int id = 0;    /* process id */
   int count = 0; /* number of solutions */
   int process = 8;
   int counter = 0;
   /*************************************************************************/

   MPI_Init(NULL, NULL);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   double startTime = 0.0, totalTime = 0.0;
   startTime = MPI_Wtime();

   for (int stage = 0; stage < 4; stage++)
   {
      process = process / 2; // 4 2 1 1/2
      for (int p = 0; p < 2 * process; p++)
      {
         if (my_rank < process) // 4 2 1 1/2
         {
            MPI_Recv(&counter, 1, MPI_INT, my_rank + process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         }
         else if (my_rank < process * 2) // 8 4 2 1
         {
            checkCircuit(p, i, &counter);
            MPI_SEND(&counter, 1, MPI_INT, my_rank + process, 0, MPI_COMM_WORLD);
         }
      }
   }

   printf("Process %d finished.\n", id); //id 即 rank
   fflush(stdout);
   printf("\nA total of %d solutions were found.\n\n", count);

   totalTime = MPI_Wtime() - startTime;
   printf("Process %d finished in time %f secs.\n", id, totalTime);
   MPI_Finalize();

   /*************************************************************************/
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

void checkCircuit(int id, int bits, int *count_ptr)
{
   int v[SIZE]; /* Each element is a bit of bits */
   int i;

   //bits
   for (i = 0; i < SIZE; i++)
      v[i] = EXTRACT_BIT(bits, i);

   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]))
   {
      printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout); //fflush(stdout)能使機器每輸出一個.暫停一秒鐘，而不會出現亂序
      count_ptr++;
   }

   //bits +1
   for (i = 0; i < SIZE; i++)
      v[i] = EXTRACT_BIT(bits + 1, i);

   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4]) && (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15]) && (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9]) && (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13]) && (v[13] || !v[14]) && (v[14] || v[15]))
   {
      printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout); //fflush(stdout)能使機器每輸出一個.暫停一秒鐘，而不會出現亂序
      count_ptr++;
   }
}
