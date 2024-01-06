#include <swim.h>

#include <intrin.h>
#include <stdio.h>

#include <jemalloc/jemalloc.h>

int main()         {
    void* mem[8192];
    u64_t begin = __rdtsc();  for (u64_t i = 0; i < 8192; ++i) if (!(mem[i] = sm_use(1024))) return -1;
    u64_t end   = __rdtsc();

    printf("%d\n", end - begin);

    begin = __rdtsc();  for (u64_t i = 0; i < 8192; ++i) mem[i] = je_malloc(1024);
    end   = __rdtsc();

    printf("%d\n", end - begin);
}