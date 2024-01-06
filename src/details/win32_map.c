#include "../map.h"
#include "../root.h"

#include <bit.h>
#include <Windows.h>

obj_trait sm_map_trait     = {
    .on_new   = &sm_map_new  ,
    .on_clone = &sm_map_clone,
    .on_ref   = 0            ,
    .on_del   = &sm_map_del  ,
    .size     = sizeof(sm_map)
};

obj_trait* sm_map_t = &sm_map_trait;

bool_t 
    sm_map_new
        (sm_map* par_map, u32_t par_count, va_list par)        {
            if (par_count == 4)                                {
                sm_root* root     = va_arg(par, sm_root*)      ;
                sm_root* root_pos = va_arg(par, u64_t)         ;
                u64_t    size     = va_arg(par, u64_t)         ;
                u64_t    map      = va_arg(par, u64_t)         ;

                if (!map)                        return false_t;
                if (!root)                       return false_t;
                if (trait_of(root) != sm_root_t) return false_t;

                if (size & mask(11))             return false_t;
                if (size < 4 kb)                 return false_t;

                par_map->root_pos = root_pos ;
                par_map->root     = root     ;

                par_map->size     = size     ;
                par_map->mask     = (u64_t)-1;
                par_map->map      = map      ;

                par_map->mem_size = shr(size, 6);
                for (u64_t i = 0 ; i < 64 ; ++i)       {
                    par_map->mem[i] = map              ;
                    map            += par_map->mem_size;
                }

                return true_t;
            }
            
            u64_t mem_size = 0;
            u64_t size     = 0;
            u64_t map      = 0;

            if (par_count != 1)  return false_t; size = va_arg(par, u64_t);
            if (size & mask(12)) return false_t;
            if (size < 4 kb)     size = 4 kb   ;

            map      = VirtualAlloc (0, size, MEM_COMMIT, PAGE_READWRITE); if (!map) return false_t;
            mem_size = shr(size, 6)                                      ;

            par_map->map = map;
            for (u64_t i = 0 ; i < 64 ; ++i) {
                par_map->mem[i] = map     ;
                map            += mem_size;
            }

            par_map->mem_size = mem_size - sizeof(sm_mem);
            par_map->size     = size                     ;
            par_map->mask     = (u64_t) -1               ;
            
            return true_t;
}


bool_t 
    sm_map_clone
        (sm_map* par, sm_map* par_clone) {
            return false_t;
}

void   
    sm_map_del
        (sm_map* par)                            {
            if (par->root)                       {
                u64_t    root_pos = par->root_pos;
                sm_root* root     = par->root    ; bts64(root->mask, root_pos);

                return;
            }

            VirtualFree(par->map, 0, MEM_DECOMMIT);
            VirtualFree(par->map, 0, MEM_RELEASE) ;
}

sm_mem*
    sm_map_use
        (sm_map* par)                              {
            if (!par)                      return 0;
            if (trait_of(par) != sm_map_t) return 0;
            if (!par->mask)                return 0;

            sm_root *root    = par->root        ;
            u64_t    ret_pos = bsr64(par->mask) ; 
            sm_map  *ret     = par->mem[ret_pos]; if (!ret) return 0;

            btr64(&par->mask, ret_pos);
            if   (!par->mask)                              {
                if (!root)                       return ret;
                if (trait_of(root) != sm_root_t) return ret;

                btr64(&root->mask, par->root_pos);
                return ret;
            }

            return ret;
}

void
    sm_map_free
        (sm_map* par, sm_mem* par_free)          {
            if (!par)                      return;
            if (!par_free)                 return;
            
            if (trait_of(par) != sm_map_t) return;
            if (par_free->map != par)      return; u64_t pos = par_free->map_pos;
            
            if (pos > 63)                  return;
            if (par->mask & shl(1, pos))   return; 
            if (!par->mask)                      {
                u64_t    root_pos = par->root_pos;
                sm_root* root     = par->root    ;

                bts64(&root->mask, par->root_pos);
            }
            
            bts64(&par->mask, pos);
}