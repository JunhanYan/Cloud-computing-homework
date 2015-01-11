/*
 This scatter example uses 3 processes. One process generates a 3-elements array and scatters one element to each of the processes.
 
 The correct answer should be: 
 % mpirun -np 3 ./scatter
  Scatter data rank =            0 : irecv=            1
  Scatter data rank =            1 : irecv=            2
  Scatter data rank =            2 : irecv=            3
*/

#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
  int isend[3], recv, rank, size, irecv;
  int iprint, itag;
  int sbuf, rbuf,ierr, i, nmax;
  int iwork[4] = {6,11,2,4};
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Status status;
  if(iwork[2] == 0) {
    iprint=6;
  }
  else {
    iprint=iwork[1];
  }
  if(iwork[0] == 0){
    nmax=100;
  }
  else{
    nmax=iwork[0];
    if(nmax <= 0) {
      if (iprint > 0) printf(" WRONG INPUT IWORK(0)= %d \n",iwork[0]);
    }
  }
  itag = 100;
  if (rank == 0) {
     sbuf = nmax;   //nmax ==6   e..... all these are useless 
     MPI_Send(&sbuf, 1, MPI_INT, 1, itag,
                   MPI_COMM_WORLD);
  }
  else if (rank == 1) {
  MPI_Recv(&rbuf, 1, MPI_INT, 0, itag,
                MPI_COMM_WORLD, &status);
  }
//the useful code 
  if(rank == 0) {
    for (i=0;i<size;i++){
        isend[i]=i+1;
    }
    
  }
//scatter must be in all process, not only in the root 
MPI_Scatter(isend, 1, MPI_INT,   
             &irecv, 1, MPI_INT, 0,
             MPI_COMM_WORLD);
  printf(" Scatter data rank = %d : irecv= %d \n",rank,irecv);

  MPI_Finalize();
  return 0;
}
