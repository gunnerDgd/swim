#include <swim.h>
#include <obj.h>

#include <intrin.h>
#include <stdio.h>

#include <jemalloc/jemalloc.h>

int main()          {
    void* mem[16384];
    sm_use   (1 kb);
    je_malloc(1 kb);

    u64_t begin = __rdtsc(); for (u64_t i = 0; i < 16384; ++i) { mem[i] = sm_use(1 kb); }
    u64_t end   = __rdtsc();

    printf("%d\n", end - begin);

    begin = __rdtsc(); for (u64_t i = 0; i < 16384; ++i) { mem[i] = je_malloc(1 kb); }
    end   = __rdtsc();

    printf("%d\n", end - begin);
}