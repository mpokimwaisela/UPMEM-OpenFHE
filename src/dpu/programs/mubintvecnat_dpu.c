#include <stddef.h>
#include <mram.h>
#include <stdio.h>

#include "helper_functions.h"

/*
    We represent a buffer to have a size of 64 KB and handle the computations on 64 bits basis after.
    The cache maximum number of bytes is 2048 (2KB) and we initialize it with that.
*/

#define BUFFER_SIZE (1 << 16)
#define CACHE_SIZE  2048

int8_t __mram_noinit mram_array_1[BUFFER_SIZE];
int8_t __mram_noinit mram_array_2[BUFFER_SIZE];
int64_t __host mram_modulus;

int main() {
    __dma_aligned int8_t local_array_1[CACHE_SIZE];
    __dma_aligned int8_t local_array_2[CACHE_SIZE];

    for (unsigned int bytes_read = 0; bytes_read < BUFFER_SIZE; bytes_read += CACHE_SIZE) {
        mram_read(&mram_array_1[bytes_read], local_array_1, CACHE_SIZE);
        mram_read(&mram_array_2[bytes_read], local_array_2, CACHE_SIZE);

        ModAddEq((int64_t*)local_array_1, (int64_t*)local_array_2, mram_modulus, CACHE_SIZE / sizeof(int64_t));

        mram_write(local_array_1, &mram_array_1[bytes_read], CACHE_SIZE);
    }

    printf("\nDone adding in the DPU\n");
}