#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main() {
    static unsigned int nthreads = 0, tid = 0;

    #pragma omp parallel private(nthreads, tid)
    {
        tid = omp_get_thread_num();
        printf("Thread #%d says:\t\'Hello, World!\'\n", tid);

        // Only master thread does this
        if(0 == tid) {
            nthreads = omp_get_num_threads();
            printf("\nNumber of threads = %d\n", nthreads);
        }
    }

    return 0;
}
