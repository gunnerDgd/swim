#ifndef __SM_MEM_H__
#define __SM_MEM_H__

#include <obj.h>

typedef struct sm_mem      {
    u64_t           map_pos;
    struct sm_map  *map    ;
}   sm_mem;

void* sm_mem_use (obj*, u64_t);
void  sm_mem_free(void*)      ;

#endif