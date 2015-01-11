/*
This programs shows how to use send/recv.

However, it is hanging before it gets to the print statement. 
*/
#include <stdio.h>
#include "mpi.h"
int main (int argc, char **argv)
{
     int rank, size, isbuf, ierr;
     int itag, myrank, irbuf;
     MPI_Init( &argc, &argv );
     MPI_Comm_rank( MPI_COMM_WORLD, &rank );
     MPI_Comm_size( MPI_COMM_WORLD, &size );
     MPI_Status status;
     isbuf=0;
     itag=1;
     if (rank == 0){             //change myrank to rank or add myrank = rank
       while (isbuf != 10) {      //if init isbuf =1 there will be a dead loop
         isbuf+=2;
       }
       MPI_Send(&isbuf,1,MPI_INT,1,itag,MPI_COMM_WORLD);
     }
     else {
       MPI_Recv(&irbuf,1,MPI_INT,0,itag,MPI_COMM_WORLD,&status);
       printf(" I RECEIVED isbuf =%d", irbuf);
     }
     MPI_Finalize();
     return 0;
}
