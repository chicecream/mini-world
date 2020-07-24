
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