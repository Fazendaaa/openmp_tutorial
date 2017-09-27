#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

const static void subdomain(float *array, int start, int end) {
    unsigned int i = 0;

    for(i=0; i < end; i++)
        array[start+i] = 123.456;
}

const static void sub(float *array, int n) {
    unsigned int this_thread = 0, n_threads = 0, chunck = 0, start = 0;

    #pragma omp parallel default(shared) private(this_thread, n_threads, chunck, start)
    {
        this_thread = omp_get_thread_num();
        n_threads = omp_get_num_threads();
        /*  size of partition   */
        chunck = n/n_threads;
        /*  satarting array index   */
        start = this_thread*chunck;
        /*  last thread may do more */
        if(this_thread == n_threads-1)
            chunck = n-start;

        printf("#%d out of %d\tstarting at: %d with chunck size of it: %d\n", this_thread, n_threads, start, chunck);
        subdomain(array, start, chunck);
    }
}

int main(int argc, char **argv) {
    unsigned int n = 1000;
    float *array = malloc(sizeof(float)*1000);

    sub(array, n);
    free(array);

    return 0;
}
