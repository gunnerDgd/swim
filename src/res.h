#ifndef __SM_RES_H__
#define __SM_RES_H__

#include "root.h"

extern obj_trait *sm_res_t;
typedef struct    sm_res  {
    obj      head  ;
    u64_t    msb   ;
    sm_root* res[6];
}   sm_res;

bool_t  sm_res_new  (sm_res*, u32_t, va_list);
bool_t  sm_res_clone(sm_res*, sm_res*)       ;
void    sm_res_del  (sm_res*)                ;

sm_mem* sm_res_use  (sm_res*, u64_t)         ;
void    sm_res_free (sm_res*, sm_mem*)       ;

#endif