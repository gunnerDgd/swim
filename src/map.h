#ifndef __SM_MAP_H__
#define __SM_MAP_H__

#include "mem.h"

extern obj_trait *sm_map_t  ;
typedef struct    sm_map    {
    obj             head    ;
    u64_t           root_pos;
    struct sm_root *root    ;

    u64_t           size    ;
    u64_t           mask    ;
    u64_t           map     ;

    u64_t           mem_size;
    sm_mem         *mem[64] ;
}   sm_map;

bool_t  sm_map_new  (sm_map*, u32_t, va_list);
bool_t  sm_map_clone(sm_map*, sm_map*)       ;
void    sm_map_del  (sm_map*)                ;

sm_mem* sm_map_use  (sm_map*)         ;
void    sm_map_free (sm_map*, sm_mem*);

#endif