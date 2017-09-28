/*
    The number of primes up to 75000 is 7393 and took it 1s to solve it in parallel and 2s sequential
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

static const unsigned int verify_subdomain(unsigned int start, unsigned int chunck) {
    unsigned int i = 0, j = 0, counter = 0;

    for(i = start; i < start+chunck; i++) {
        for(j = 2; j < i; j++)
            if(0 == i%j)
                break;
        
        if(j == i)
            counter += 1;
    }

    return counter;
}

static const unsigned int n_primes_parallel(unsigned int limit) {
    /*  starts with one because number two is prime */
    unsigned int i = 0, j = 0, counter = 1, this_thread = 0, n_threads = 0, start = 0, chunck = 0;
    
    /*  Dynamic don't show good results to this particular case */
    omp_set_dynamic(0);
    #pragma omp parallel default(shared) private(this_thread, n_threads, chunck, start) num_threads(8)
    {
        this_thread = omp_get_thread_num();
        n_threads = omp_get_num_threads();
        chunck = limit/n_threads;
        /*  plus 3 because there's no need to cheking out the following numbers: 0, 1, 2  */
        start = 3+(this_thread*chunck);

        if(n_threads-1 == this_thread)
            chunck = limit-start;
        
        counter += verify_subdomain(start, chunck);
        printf("#%d out of %d\tstart at %d and checking up to %d\n", this_thread, n_threads, start, start+chunck);
    }

    return counter;
}

static const unsigned int n_primes_sequential(unsigned int limit) {
    unsigned int i = 0, j = 0, counter = 1;
    
    for(i = 3; i < limit; i++) {
        for(j = 2; j < i; j++)
            if(0 == i%j)
                break;
        
        if(j == i)
            counter += 1;
    }
            
    return counter;
}

int main(int argc, char **argv) {
    /*  Above that number the parallel doesn't performe better than the sequential  */
    const unsigned int limit = 75000;
    unsigned int primes_parallel = 0, primes_sequential = 0;
    float start_parallel = 0.0, end_parallel = 0.0, start_sequential = 0.0, end_sequential = 0.0;

    start_parallel = clock()/CLOCKS_PER_SEC;
    primes_parallel = n_primes_parallel(limit);
    end_parallel = clock()/CLOCKS_PER_SEC;

    start_sequential = clock()/CLOCKS_PER_SEC;
    primes_sequential = n_primes_sequential(limit);
    end_sequential = clock()/CLOCKS_PER_SEC;

    if(primes_parallel == primes_sequential)
        printf("\nThe number of primes up to %d is %d and took it %.0fs to solve it in parallel and %.0fs sequential\n", limit, primes_sequential, end_parallel - start_parallel, end_sequential - start_sequential);

    else
        printf("\nERROR\tWrong number of primes up to %d calculated:\n\tParallel: %d\n\tSequential: %d\n", limit, primes_parallel, primes_sequential);

    return 0;
}
