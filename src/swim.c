#include "swim.h"

#include "mem.h"
#include "res.h"

sm_res res;

void* 
    sm_use
        (u64_t par)                         {
            if (trait_of(&res) != sm_res_t) {
                if (!make_at(&res, sm_res_t) from(1, 4 mb))
                    return 0;
            }

            u8_t*  ret = sm_res_use(&res, par); if (!ret) return 0;
            return ret + sizeof(sm_mem)       ;
}

void
    sm_free
        (void* par)                               {
            if (trait_of(&res) != sm_res_t) return;
            sm_mem_free(par);
}