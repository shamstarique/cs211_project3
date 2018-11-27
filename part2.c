#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MIN(a,b)  ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)  ( BLOCK_LOW((id)+1,p,n)-1 ) 
#define BLOCK_SIZE(id,p,n)  (BLOCK_LOW( (id)+1, p, n) - BLOCK_LOW( (id), p, n  ) )
#define BLOCK_OWNER(index,p,n)   ( ( ((p)*(index)+1)-1 ) / (n) )

int main (int argc, char *argv[]){
    
    int id, p,  count;
    double elapsed_time;
    char * marked;
    char*  local_marked;
    long long int i,n, k,index, size,global_count, low_value, high_value,proc_0, proc0_size,prime,first, local_size;

    MPI_Init (&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    
    global_count = 0;

    if (argc != 2) {
          if (!id) printf ("Command line: %s <m>\n", argv[0]);
          MPI_Finalize(); exit (1);
    }

    n = atoll(argv[1]);
    low_value = 3 + BLOCK_LOW(id,p,n-2) + BLOCK_LOW(id,p,n-2)%2;
    high_value = 3 + BLOCK_HIGH(id,p,n-2) - BLOCK_HIGH(id,p,n-2)%2;
    size = ((high_value-low_value)/2)+1;
    proc0_size = (n-2)/(p*2);
    
    if ((3 + proc0_size) < (int) sqrt((double) n)) {
        if (!id) printf ("Too many processes\n");
            MPI_Finalize();
            exit (1);
    }
   
//added this to make sure each procces of the program finds its own sieving primes via local computations instead of broadcasts 
    local_size = sqrt(n)+1;
    local_marked = (char*)malloc(local_size);
    k=2;

    for(i =0; i<local_size;i++) local_marked[i] = 0;
    
    do{
        for(i=k*k;i<local_size; i++)
            if(i%k ==0)
                local_marked[i] = 1;
        while(local_marked[++k]);
    }while(k*k <= local_size);


    
    marked = (char *) malloc (size);
    if (marked == NULL) {
        printf ("Cannot allocate enough memory\n");
            MPI_Finalize();
            exit (1);
    }
    
    for (i = 0; i < size; i++) marked[i] = 0;
    
    index = 2;
    prime = 3;
        
    do {
        if (prime * prime > low_value)
            first = (prime * prime - low_value)/2;
            else {
                if (!(low_value % prime)) first = 0;
                else first = (prime - low_value % prime +low_value/prime%2 * prime)/2;
            }
            for (i = first; i < size; i += prime) marked[i] = 1;
                
            
            while (local_marked[++index]);
            prime = index;
            
           // if(p>1) MPI_Bcast (&prime,  1, MPI_INT, 0, MPI_COMM_WORLD); //no need to broadcast anymre each process does own work and does not divy up work
       } while (prime * prime <= n);

    count = 0;
    for (i = 0; i < size; i++)
        if (!marked[i]) count++;
   
    if(p>1) MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
    elapsed_time += MPI_Wtime();
    if (!id) {
          global_count++;
          printf ("%d primes are less than or equal to %d\n", global_count, n);
          printf ("Total elapsed time: %10.6f\n", elapsed_time);
        }
    MPI_Finalize ();

    return 0;
}
