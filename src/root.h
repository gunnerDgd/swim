#ifndef __SM_ROOT_H__
#define __SM_ROOT_H__

#include "map.h"

extern obj_trait *sm_root_t ;
typedef struct    sm_root   {
    obj             head    ;
    struct sm_root *next    ;
    struct sm_res  *res     ;

    u64_t           size    ;
    u64_t           mask    ;
    u64_t           root    ;

    u64_t           map_size;
    sm_map          map[64] ;
}   sm_root;

bool_t   sm_root_new  (sm_root*, u32_t, va_list);
bool_t   sm_root_clone(sm_root*, sm_root*)      ;
void     sm_root_del  (sm_root*)                ;

sm_mem*  sm_root_use  (sm_root*)                ;
void     sm_root_free (sm_root*, sm_mem*)       ;
sm_root* sm_root_next (sm_root*, sm_root*)      ;

#endif