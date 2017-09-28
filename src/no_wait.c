/*
    If there are multiple independent loops within a parallel region, the nowait
    clause avoid the implied barrier at the ende of the loop construct.

        Parallel benchmark: 4.00s
        Sequential benchmark: 2.00s
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

const static void no_wait_example(unsigned int n, unsigned int m, float *a, float *b, float *y, float *z) {
    #pragma omp parallel
    {
        #pragma omp for nowait
            for(unsigned int i = 1; i < n; i++)
                b[i] = (a[i] + a[i-1])/2.0;

        #pragma omp for nowait
            for(unsigned int i = 0; i < m; i++)
                y[i] = sqrt(z[i]);
    }
}

const static void without_no_wait(unsigned int n, unsigned int m, float *a, float *b, float *y, float *z) {
    for(unsigned int i = 1; i < n; i++)
        b[i] = (a[i] + a[i-1])/2.0;
    for(unsigned int i = 0; i < m; i++)
        y[i] = sqrt(z[i]);
}

static float *initializer(unsigned int limit) {
    float *array = malloc(sizeof(float) * limit);
    float counter = 0.0;

    if(NULL != array) {
        for(unsigned int i = 0; i < limit; i++) {
            array[i] = counter;
            counter += 1;
        }
    }

    return array;
}

int main(int argc, char **argv) {
    static float start_time_parallel = 0.0, end_time_parallel = 0.0, start_time_sequential = 0.0, end_time_sequential = 0.0;
    static float *a = NULL, *b = NULL, *c = NULL, *d = NULL;
    /*  DO NOT TRY TO PUSH THIS NUMBER EVEN MORE    */
    const static unsigned int n = 250000000;

    a = initializer(n);
    b = initializer(n);
    c = initializer(n);
    d = initializer(n);
    
    start_time_parallel = clock()/CLOCKS_PER_SEC;
    no_wait_example(n, n, a, b, c, d);
    end_time_parallel = clock()/CLOCKS_PER_SEC;
    
    free(a);
    free(b);
    free(c);
    free(d);

    printf("\nParallel benchmark: %.2fs\n", end_time_parallel - start_time_parallel);
    
    a = initializer(n);
    b = initializer(n);
    c = initializer(n);
    d = initializer(n);

    start_time_sequential = clock()/CLOCKS_PER_SEC;
    without_no_wait(n, n, a, b, c, d);
    end_time_sequential = clock()/CLOCKS_PER_SEC;

    printf("Sequential benchmark: %.2fs\n", end_time_sequential - start_time_sequential);
    
    free(a);
    free(b);
    free(c);
    free(d);
}
