#include "swim.h"

#include "mem.h"
#include "res.h"

sm_res res;

void* 
    sm_use
        (u64_t par)                         {
            if (trait_of(&res) != sm_res_t) {
                if (!make_at(&res, sm_res_t) from(0))
                    return 0;
            }

            return sm_res_use(&res, par);
}

void
    sm_free
        (void* par)                               {
            if (trait_of(&res) != sm_res_t) return;
            sm_mem_free(par);
}