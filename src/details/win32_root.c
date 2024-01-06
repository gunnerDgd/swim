#include "../root.h"
#include "../res.h"

#include <bit.h>
#include <Windows.h>

obj_trait sm_root_trait     = {
    .on_new   = &sm_root_new  ,
    .on_clone = &sm_root_clone,
    .on_ref   = 0             ,
    .on_del   = &sm_root_del  ,
    .size     = sizeof(sm_root)
};

obj_trait *sm_root_t = &sm_root_trait;

bool_t 
    sm_root_new
        (sm_root* par_root, u32_t par_count, va_list par) {
            u64_t size = 0;
            if (par_count == 0) size = 256 kb            ;
            if (par_count == 1) size = va_arg(par, u64_t);
            if (size < 256 kb)  size = 256 kb            ;

            u64_t   root     = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE); if (!root) return false_t;
            u64_t   map_size = shr(size, 6) ;
            sm_map *map      = par_root->map;

            par_root->root = root;
            for (u64_t i = 0 ; i < 64 ; ++i)                  {
                bool_t res = make_at (&map[i], sm_map_t) from (
                    4                   ,
                    par_root            ,
                    i                   ,
                    map_size            ,
                    root
                );

                if (!res)                                       {
                    VirtualFree(par_root->root, 0, MEM_DECOMMIT);
                    VirtualFree(par_root->root, 0, MEM_RELEASE) ;
                    return false_t;
                }

                root += map_size;
            }

            par_root->mask     = (u64_t)-1;
            par_root->map_size = map_size ;
            par_root->size     = size     ;
            return true_t;
}

bool_t 
    sm_root_clone
        (sm_root* par, sm_root* par_clone) {
            return false_t;
}

void   
    sm_root_del
        (sm_root* par)                                       {
            for(u64_t i = 0 ; i < 64 ; ++i) del(&par->map[i]);

            VirtualFree(par->root, 0, MEM_DECOMMIT);
            VirtualFree(par->root, 0, MEM_RELEASE) ;
}

sm_mem*
    sm_root_use
        (sm_root* par)                              {
            if (!par)                       return 0;
            if (trait_of(par) != sm_root_t) return 0;
            if (!par->mask)                 return 0;

            u64_t   pos = bsf64(par->mask); if (pos > 63)       return 0;
            sm_map *map = &par->map[pos]  ; if (!trait_of(map)) return 0;

            return sm_map_use(map);
}

void  
    sm_root_free
        (sm_root* par, sm_mem* par_free)         {
            if (!par)                      return;
            if (trait_of(par) != sm_map_t) return;
            if (!par_free)                 return; sm_map* map = par_free->map;
            
            if (!map)                      return;
            if (map->root != par)          return;

            sm_map_free(map, par_free);
}