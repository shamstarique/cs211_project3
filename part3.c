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
    
    int id, p;
    double elapsed_time;
    char * marked;
    char*  local_marked;
    long long int i, n, k, count, index, size,global_count, low_value, high_value,proc_0, proc0_size,prime, local_prime,first,local_first, local_size;
    
    long long int size_of_block = 1000000;
    
    
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
    low_value = 2 + BLOCK_LOW(id,p,n-1) + (BLOCK_LOW(id,p,n-1)+3)%2;
    high_value = 2 + BLOCK_HIGH(id,p,n-1) - (BLOCK_HIGH(id,p,n-1)+3)%2;
    size = ((high_value-low_value)/2)+1;
    marked = (char *) malloc (size);
    local_size = (int)sqrt((double)n) -1;
    local_marked = (char*)malloc(local_size);
    k=2;
    proc0_size = (n-2)/(p*2);
  
    if ((3 + proc0_size) < (int) sqrt((double) n/2)) {
        if (!id) printf ("Too many processes\n");
            MPI_Finalize();
            exit (1);
    }
   
    
    if (marked == NULL || local_marked == NULL) {
        printf ("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit (1);
    }
    for(i =0; i<local_size;i++) local_marked[i] = 0;
    
    do{
        local_first = k*k -2;
        for(i=local_first; i< local_size; i+=k) local_marked[i] =1;
        while(local_marked[++index]);
        k = 2+index;
    }while( k*k <=n);

    
    
    for (i = 0; i < size; i++) marked[i] = 0;
    
    long long int low_value_in_cache = low_value;
    long long int high_value_in_cache = low_value_in_cache +2 *(size_of_block-1);
            
    do {
        index = 2; 
        prime = 3;
        
        while(prime*prime <= high_value_in_cache){
            if(prime*prime>low_value_in_cache){
                 first = (prime*prime - low_value_in_cache)/2;
            }else{
                if(!(low_value_in_cache %prime)) first = 0;
                else first = (prime - low_value_in_cache%prime + low_value_in_cache / prime%2 * prime)/2;
            }
            
            for(i = first +(low_value_in_cache - low_value)/2; i <= (high_value_in_cache - low_value )/2; i = i+prime) marked[i] = 1;
            while(local_marked[++index]);
           
            prime = 2 + index;
  
    
        }
        low_value_in_cache = high_value_in_cache +2;
        high_value_in_cache = low_value_in_cache + 2*(size_of_block-1);        
        if(high_value_in_cache > high_value) high_value_in_cache = high_value;
           
    } while (low_value_in_cache <= high_value);

    count = 0;
    for (i = 0; i < size; i++)
        if (!marked[i]) count++;

    if(!id) count++;
   
    if(p>1) MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   
    elapsed_time += MPI_Wtime();
    
    global_count--; //somewhere i am counting either the # 1 or something TODO: solve this by printing out 10^2 primes to see what i am getting
    if (!id) {
          global_count++;
          printf ("%d primes are less than or equal to %d\n", global_count, n);
          printf ("Total elapsed time: %10.6f\n", elapsed_time);
        }
    MPI_Finalize ();

    return 0;
}
