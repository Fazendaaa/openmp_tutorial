/*
    Time to solve it for 250000000 numbers:
        Parallel: 6s
        Sequential: 4
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <omp.h>

static void no_wait_example_2(const unsigned int n, float *a, float *b, float *c, float *y, float *z) {
    unsigned int i = 0;

    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
            for(i = 0; i < n; i++)
                c[i] = (a[i] + b[i])/2.0f;
    
        #pragma omp for schedule(static) nowait
            for(i = 0; i < n; i++)
                z[i] = sqrtf(c[i]);
    
        #pragma omp for schedule(static) nowait
            for(i = 1; i < n; i++)
                y[i] = z[i-1] + a[i];
    }
}

static void without_no_wait_example_2(const unsigned int n, float *a, float *b, float *c, float *y, float *z) {
    unsigned int i = 0;

    for(i = 0; i < n; i++)
        c[i] = (a[i] + b[i])/2.0f;
    
    for(i = 0; i < n; i++)
        z[i] = sqrtf(c[i]);

    for(i = 1; i < n; i++)
        y[i] = z[i-1] + a[i];
}

static float *initializer(unsigned int limit) {
    float *array = malloc(sizeof(float) * limit);
    float counter = 1.0;

    if(NULL != array) {
        for(unsigned int i = 0; i < limit; i++) {
            array[i] = counter;
            counter += 1;
        }
    }

    return array;
}

int main(int argc, char **argv) {
    const unsigned int n = 250000000;
    float start_parallel = 0, end_parallel = 0, start_sequential = 0, end_sequential = 0;
    float *a = NULL, *b = NULL, *c = NULL, *y = NULL, *z = NULL;

    a = initializer(n);
    b = initializer(n);
    c = initializer(n);
    y = initializer(n);
    z = initializer(n);

    start_parallel = clock()/CLOCKS_PER_SEC;
    no_wait_example_2(n, a, b, c, y, z);
    end_parallel = clock()/CLOCKS_PER_SEC;

    free(a);
    free(b);
    free(c);
    free(y);
    free(z);

    a = initializer(n);
    b = initializer(n);
    c = initializer(n);
    y = initializer(n);
    z = initializer(n);

    start_sequential = clock()/CLOCKS_PER_SEC;
    without_no_wait_example_2(n, a, b, c, y, z);
    end_sequential = clock()/CLOCKS_PER_SEC;

    free(a);
    free(b);
    free(c);
    free(y);
    free(z);

    printf("\nTime to solve it for %d numbers:\n\tParallel: %.0fs\n\tSequential: %.0f\n", n, end_parallel - start_parallel, end_sequential - start_sequential);

    return 0;
}
