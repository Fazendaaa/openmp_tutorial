/*
    In this case when running with no dynamic threads:

        Parallel time: 7.000000
        Sequential time: 9.000000

    When running with dynamic threads:

        Parallel time: 11.000000
        Sequential time: 9.000000
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

const static void power(double *array, double factor, unsigned int start, unsigned int chunck) {
    for(unsigned int i = 0; i < chunck; i++)
        array[start+i] = pow(array[start+i], factor);
}


int main(int argc, char **argv) {
    unsigned int this_thread = 0, n_threads, n = 100000000, chunck = 0, start = 0, end = 0;
    double counter = 0.0;
    double *array_parallel = malloc(sizeof(double) * n), *array_sequential = malloc(sizeof(double) * n);
    float start_parallel = 0, end_parallel = 0, start_sequential = 0, end_sequential = 0;

    for(unsigned int i = 0; i < n; i++) {
        array_parallel[i] = counter;
        array_sequential[i] = counter;
        counter += 1.0;
    }

    start_parallel = clock()/CLOCKS_PER_SEC;
    
    /*  FALSE   */
    // omp_set_dynamic(0);
    /*  TRUE   */
    omp_set_dynamic(1);
    #pragma omp parallel num_threads(10)
    {
        n_threads = omp_get_num_threads();
        this_thread = omp_get_thread_num();
        chunck = n/n_threads;
        start = this_thread*chunck;

        if(n_threads-1 == this_thread)
            chunck = n - start;

        printf("#%d thread doing pow for index %d to %d\n", this_thread, start, chunck);
        power(array_parallel, 3.0, start, chunck);
    }
    end_parallel = clock()/CLOCKS_PER_SEC;

    start_sequential = clock()/CLOCKS_PER_SEC;
    power(array_sequential, 3.0, 0, n);
    end_sequential = clock()/CLOCKS_PER_SEC;

    printf("\nParallel time: %f\n", end_parallel - start_parallel);
    printf("Sequential time: %f\n", end_sequential - start_sequential);

    free(array_parallel);
    free(array_sequential);

    return 0;
}
