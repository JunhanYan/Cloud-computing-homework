#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
#include<omp.h>
#include<memory.h>
#include<math.h>
#include<time.h>
int* createRamVector(int *sendbuffer,int sizeOfVector)
{
	sendbuffer = (int *) malloc (sizeOfVector * sizeof(int));
	srand((unsigned)time(NULL));
	int i ;
	#pragma omp parallel for
	for (i=0; i<sizeOfVector; i++)
	{
         sendbuffer[i] = rand() % 10 + 1; // the random numbers are between 1 to 10
    }
	return 	sendbuffer;
}

int main(int argc, char *argv[])
{
	if(argc!=2){
		printf("Please input the vector size!.\n");
		exit(1);
	}
	 int sizeOfVector = atoi(argv[1]); 
	 int size, rank;
	 int *sendbuffer, *recvbuffer;
	 int i;
	 
	 MPI_Init(&argc, &argv); //MPI_Init
	 MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get rank of the processes
	 MPI_Comm_size(MPI_COMM_WORLD, &size); //get size 
	
	 int recvsize = sizeOfVector % size == 0 ? sizeOfVector / size : (int)(sizeOfVector / size) + 1;//chunk size
	 recvbuffer = (int*) malloc (sizeof(int) * recvsize);
	 if(rank == 0)
	 {
		sendbuffer = createRamVector(sendbuffer, sizeOfVector);
		printf("Random vector is:\n");
		#pragma omp parallel for
		for(i=0; i<sizeOfVector; i++)
		{
			printf("  %d", sendbuffer[i]);
		}
	 }   
	 // distribute chunks to all processes
	 MPI_Scatter(sendbuffer, recvsize, MPI_INT, recvbuffer, recvsize, MPI_INT, 0, MPI_COMM_WORLD);

	 long product= 1;
	 long result=1;
    		
    
	 // conduct product for the chunk of elements on each process
	 if(rank < (int)(sizeOfVector / recvsize))
	 {	
		#pragma omp parallel for reduction(*:product)
		for(i=0; i<recvsize; i++)
		{
		    product *= recvbuffer[i];
		}
	 }
	 else if(rank == (int)(sizeOfVector / recvsize))
	 {
		#pragma omp parallel for reduction(*:product)
		for(i=0; i<sizeOfVector - recvsize * (size - 1); i++)
		{
		    product *= recvbuffer[i];
		}
	 }
 
	  // reduce final result at master node
	  MPI_Reduce(&product, &result, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
	  if(rank == 0)
	  {
		printf("\nThe geometric mean is %lf\n", pow(result * 1.0, 1.0 / sizeOfVector));
	  }
	  MPI_Finalize();
	  return 0;					  					 

}
