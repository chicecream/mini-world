
#define PERL_NO_GET_CONTEXT
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#define NEED_sv_2pv_flags
#define NEED_vnewSVpvf
#define NEED_warner
#include "ppport.h"

#include "const-c.inc"

/*#define DEBUG*/
#define PERL_VERSION_DECIMAL(r,v,s) (r*1000000 + v*1000 + s)
#define PERL_DECIMAL_VERSION \
  PERL_VERSION_DECIMAL(PERL_REVISION,PERL_VERSION,PERL_SUBVERSION)
#define PERL_VERSION_GE(r,v,s) \
  (PERL_DECIMAL_VERSION >= PERL_VERSION_DECIMAL(r,v,s))

#ifndef UNLIKELY
#  define UNLIKELY(x) (x)
#endif
#ifndef LIKELY
#  define LIKELY(x) (x)
#endif

#ifndef OpSIBLING
#define OpSIBLING(o) o->op_sibling
#endif

#ifndef OpMORESIB_set
#define OpMORESIB_set(o, sib) (o->op_sibling = sib)
#endif

#define OPT_MY 1
#define OPT_ALIAS 2

#ifdef DEBUG
static void analyse_op_tree(pTHX_ OP *o, int depth){
    for(int i=0; i<depth; ++i)
        printf("  ");
    printf("%s=%p op_next=%p op_flags=%X op_private=%X\n", OP_NAME(o), (void*)o, (void*)o->op_next, o->op_flags, o->op_private);
    if( o->op_flags & OPf_KIDS )
        for(OP *kid=cUNOPo->op_first; kid; kid=OpSIBLING(kid))
            analyse_op_tree(aTHX_ kid, depth+1);
}
#endif

static int sv_alias_get(pTHX_ SV* sv, MAGIC *mg){
#ifdef DEBUG
    puts("sv_alias_get");
#endif
    sv_setsv_flags(sv, mg->mg_obj, SV_GMAGIC);
    return 0;
}
static int sv_alias_set(pTHX_ SV* sv, MAGIC *mg){
#ifdef DEBUG
    puts("sv_alias_set");
#endif
    sv_setsv_flags(mg->mg_obj, sv, 0);
    SvSETMAGIC(mg->mg_obj);
    return 0;
}
static MGVTBL sv_alias_vtbl = {
    sv_alias_get,
    sv_alias_set,
    (U32 (*)(pTHX_ SV*, MAGIC*)) NULL,
    (int (*)(pTHX_ SV*, MAGIC*)) NULL,
    (int (*)(pTHX_ SV*, MAGIC*)) NULL
};

static void prepare_anonlist_node(pTHX_ OP * parent, OP * o, U32 opt);
static void prepare_anonhash_node(pTHX_ OP * parent, OP * o, U32 opt);

static inline void my_sv_set(pTHX_ SV ** dst, SV ** src, U32 is_alias){
    if( src ){
        if( is_alias ){
            sv_magicext(*dst, *src, PERL_MAGIC_ext, &sv_alias_vtbl, NULL, 0);
        }
        else{
            SvGETMAGIC(*src);
            SvSetMagicSV_nosteal(*dst, *src);
        }
    }
    else{
        if( is_alias ){
            warn("take alias on a non-exist magic element");
            SvSetSV(*dst, &PL_sv_undef);
        }
        else{
            SvSetMagicSV(*dst, &PL_sv_undef);
        }
    }
}

static inline int anonlist_set_common(pTHX_ SV * sv, MAGIC * mg, U32 opt){
    SV ** list_holder = (SV**)(mg->mg_ptr + sizeof(I32*));
    I32 * const_index = *(I32**)mg->mg_ptr;
    I32 nitems = (mg->mg_len - sizeof(I32*)) / sizeof(SV*);

#ifdef DEBUG
    printf("anonlist_set opt=%u, nitems=%d\nconst_index =", (unsigned int)opt, (int)nitems);
    for(I32 i=0; const_index[i]<nitems; ++i)
        printf(" %d", const_index[i]);
    printf(" %d\n", nitems);
#endif

    if( !SvROK(sv) ){
        warn("assign non-ref value but SvTYPE=%d to a list pattern", SvTYPE(sv));
        return 0;
    }

    SV * src = SvRV(sv);
    if( SvTYPE(src)!=SVt_PVAV ){
        warn("assign non array ref value but a ref of SvTYPE=%d to a list pattern", SvTYPE(SvRV(sv)));
        return 0;
    }
