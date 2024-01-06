#include "mem.h"

#include "map.h"
#include "root.h"
#include "res.h"

void* 
    sm_mem_use
        (obj* par, u64_t par_size) {
            u8_t *ret = 0;
            if (trait_of(par) == sm_root_t) ret = sm_root_use(par)          ;
            if (trait_of(par) == sm_map_t)  ret = sm_map_use (par)          ;
            if (trait_of(par) == sm_res_t)  ret = sm_res_use (par, par_size);
            if (!ret)                       return 0                        ;

            return ret + sizeof(sm_mem);
}

void  
    sm_mem_free
        (void* par)                                    {
            if (!par)                            return; sm_mem* free = (u8_t*)par - sizeof(sm_mem);
            if (!free->map)                      return;
            if (trait_of(free->map) != sm_map_t) return;
            
            sm_map_free(free->map, free);
}