#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv) {
    omp_set_nested(1);
    omp_set_dynamic(0);
    // omp_set_num_threads(8);

    #pragma omp parallel
    {
        #pragma omp parallel
        {
            /*
                If OMP_NUM_TRHEADS = x was set, the following should print:
                #1 Inner    #threads: x
                #1 Inner    #threads: x
                 .
                 .
                 .
                #1 Inner    #threads: x

                If nesting is not supported, the following should print:
                #1 Inner    #threads: 1
                #1 Inner    #threads: 1
            */
            #pragma omp single
            {
                printf("#1 Inner\t#threads: %d\n", omp_get_num_threads());
            }
        }

        #pragma omp barrier
        omp_set_nested(0);
        #pragma omp parallel
        {
            /*
                Even if OMP_NUM_THREADS = x was set, the following should print,
                bcause nesting is disable:
                #1 Inner    #threads: 1
                #1 Inner    #threads: 1
                 .
                 .
                 .
                #1 Inner    #threads: 1
            */
            #pragma omp single
            {
                printf("#2 Inner\t#threads: %d\n", omp_get_num_threads());                
            }
        }

        #pragma omp barrier
        /*
            If OMP_NUM_THREADS = x was set, the following should print:
            Outer   #threads: x
        */
        #pragma omp single
        {
            printf("Outer\t#threads: %d\n", omp_get_num_threads());                
        }
    }

    return 0;
}
