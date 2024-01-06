#include "../res.h"

#include <bit.h>
#include <Windows.h>

sm_root*
    sm_res_do_expand
        (sm_res* par, u64_t par_pos)                     {
            if (!par)                      return false_t;
            if (trait_of(par) != sm_res_t) return false_t;

            u64_t    root_size = shl(1, (par->msb + par_pos))                                  ;
            sm_root* root      = VirtualAlloc (0, sizeof(sm_root),  MEM_COMMIT, PAGE_READWRITE);

            if (!root)                                          return false_t;
            if (!make_at (root, sm_root_t) from (1, root_size)) {
                VirtualFree(root, 0, MEM_DECOMMIT);
                VirtualFree(root, 0, MEM_RELEASE) ;
                return false_t;
            }

            root->res  = par              ;
            root->next = par->res[par_pos]; par->res[par_pos] = root;
            return root;
}

void
    sm_res_do_shrink
        (sm_res* par)                       {
            for (u64_t i = 0 ; i < 6 ; ++i) {
                sm_root* root = par->res[i];
                while   (root)                     {
                    sm_root* root_next = root->next;
                    if (root->mask)     {
                        root = root_next;
                        continue;
                    }

                    del(root)                         ;
                    VirtualFree(root, 0, MEM_DECOMMIT);
                    VirtualFree(root, 0, MEM_RELEASE) ;

                    root = root_next;
                }
            }
}

obj_trait sm_res_trait     = {
    .on_new   = &sm_res_new  ,
    .on_clone = &sm_res_clone,
    .on_ref   = 0            ,
    .on_del   = &sm_res_del  ,
    .size     = sizeof(sm_res)
};

obj_trait *sm_res_t = &sm_res_trait;

bool_t 
    sm_res_new
        (sm_res* par_res, u32_t par_count, va_list par)       {
            u64_t size = 0                                    ;
            if      (par_count == 0) size = 256 kb            ;
            else if (par_count == 1) size = va_arg(par, u64_t);
            else                     return false_t           ;

            if (size < (256 kb))    size = 256 kb ;
            if (size & mask(18ull)) return false_t;

            for (u64_t i = 0 ; i < 6 ; ++i)                                                  {
                sm_root* root = VirtualAlloc (0, sizeof(sm_root), MEM_COMMIT, PAGE_READWRITE);

                if (!root)                                             return false_t;
                if (!make_at(root, sm_root_t) from (1, shl(size, i))) {
                    VirtualFree(root, 0, MEM_DECOMMIT);
                    VirtualFree(root, 0, MEM_RELEASE) ;

                    return false_t;
                }

                root->res = par_res   ;
                par_res->res[i] = root;
            }

            par_res->msb = bsr64(size) - 12;
            return true_t;
}

bool_t 
    sm_res_clone
        (sm_res* par, sm_res* par_clone) {
            return false_t;
}

void
    sm_res_del
        (sm_res* par)                       {
            for (u64_t i = 0 ; i < 6 ; ++i) {
                sm_root* root = par->res[i];
                while   (root)                        {
                    sm_root*    root_next = root->next;
                    del        (root)                 ;
                    VirtualFree(root, 0, MEM_DECOMMIT);
                    VirtualFree(root, 0, MEM_RELEASE) ;

                    root = root_next;
                }
            }
}

sm_mem*
    sm_res_use
        (sm_res* par, u64_t par_size)                    {
            if (!par)                      return false_t;
            if (trait_of(par) != sm_res_t) return false_t; u64_t pos = bsr64(par_size);

            if (pos < par->msb) pos = par->msb; pos -= par->msb;
            if (pos > 5)        return false_t;

            sm_mem* ret = 0;
            ret = sm_root_use(par->res[pos])              ; if (ret) return ret;
            ret = sm_root_use(sm_res_do_expand(par, pos));

            return ret;
}

void
    sm_res_free
        (sm_res* par, sm_mem* par_free)          {
            if (!par)                      return;
            if (!par_free)                 return;
            if (trait_of(par) != sm_res_t) return; sm_map* map = par_free->map;

            if (!map)                  return;
            if (!map->root)            return;
            if (map->root->res != par) return;

            sm_map_free(par_free->map, par_free);
}