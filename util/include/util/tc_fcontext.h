#pragma once

namespace tars
{


typedef void*   fcontext_t;

struct transfer_t {
    fcontext_t  fctx;
    void    *   data;
};

extern "C" transfer_t tars_jump_fcontext( fcontext_t const to, void * vp);
extern "C" fcontext_t tars_make_fcontext( void * sp, std::size_t size, void (* fn)( transfer_t) );

}
