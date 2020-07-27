
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

    I32 key = 0;
    for(I32 i=0; i<nitems; ++i, ++list_holder){
        if( i==*const_index ){
            if( SvOK(*list_holder) )
                key = (I32) SvIV(*list_holder);
            else
                ++key;
            ++const_index;
        }
        else{
            switch( SvTYPE(*list_holder) ){
                case SVt_PVAV:
                    {
                        AV *dst = (AV*)(*list_holder);
                        int magic = SvMAGICAL(dst) != 0;
                        I32 last_key = key < 0 ? -1 : AvFILL((AV*)src);

                        ENTER;
                        SAVEFREESV(SvREFCNT_inc_simple_NN((SV*)dst));
                        av_clear(dst);
                        av_extend(dst, last_key+1-key);
                        I32 j = 0;
                        while( key <= last_key ){
                            SV ** ptr_val = av_fetch((AV*)src, key, 0);
                            SV * new_sv = newSV(0);
                            my_sv_set(aTHX_ &new_sv, ptr_val, i != -*const_index-1 && opt & OPT_ALIAS);
                            SV ** didstore = av_store(dst, j, new_sv);
                            if( magic ){
                                if( !didstore )
                                    sv_2mortal(new_sv);
                                SvSETMAGIC(new_sv);
                            }
                            ++j;
                            ++key;
                        }
#if PERL_VERSION_GE(5,14,0)
                        if( PL_delaymagic & DM_ARRAY_ISA )
                            SvSETMAGIC(*list_holder);
#endif
                        LEAVE;
                    }
                    break;
                case SVt_PVHV:
                    {
                        HV *dst = (HV*)(*list_holder);
                        int magic = SvMAGICAL(dst) != 0;
                        I32 last_key = key < 0 ? -1 : AvFILL((AV*)src);

                        if( key <= last_key && ((last_key - key) & 1) == 0 )
                            Perl_warner(aTHX_ packWARN(WARN_MISC), "Odd number of elements in hash assignment");

                        ENTER;
                        SAVEFREESV(SvREFCNT_inc_simple_NN((SV*)dst));
                        hv_clear(dst);
                        while( key <= last_key ){
                            SV ** ptr_key = av_fetch((AV*)src, key, 0);
                            SV ** ptr_val = key < last_key ? av_fetch((AV*)src, key+1, 0) : NULL;
                            SV * new_key;
                            if( ptr_key )
                                if( SvGMAGICAL(*ptr_key) )
                                    new_key = sv_mortalcopy(*ptr_key);
                                else
                                    new_key = *ptr_key;
                            else
                                new_key = newSV(0);
                            SV * new_val = newSV(0);
                            my_sv_set(aTHX_ &new_val, ptr_val, i != -*const_index-1 && opt & OPT_ALIAS);
                            HE * didstore = hv_store_ent(dst, new_key, new_val, 0);
                            if( magic ){
                                if( !didstore )
                                    sv_2mortal(new_val);
                                SvSETMAGIC(new_val);
                            }
                            key += 2;
                        }
                        LEAVE;
                    }
                    break;
                default:
                    {
                        SV ** ptr_val = av_fetch((AV*)src, key, 0);
                        my_sv_set(aTHX_ list_holder, ptr_val, (i != -*const_index-1 && opt & OPT_ALIAS));
                    }
            }
            if( i == -*const_index-1 )
                ++const_index;
            ++key;
        }
    }
    return 0;
}
static int anonlist_set(pTHX_ SV * sv, MAGIC * mg){
    return anonlist_set_common(aTHX_ sv, mg, 0);
}
static int anonlist_alias_set(pTHX_ SV * sv, MAGIC * mg){
    return anonlist_set_common(aTHX_ sv, mg, OPT_ALIAS);
}

static inline int anonhash_set_common(pTHX_ SV * sv, MAGIC * mg, U32 opt){
    SV * src;
    char *key = "";
    STRLEN keylen = 0;
    SV ** list_holder = (SV**)(mg->mg_ptr + sizeof(I32*));
    I32 * const_index = *(I32**)mg->mg_ptr;
    I32 nitems = (mg->mg_len - sizeof(I32*)) / sizeof(SV*);

#ifdef DEBUG
    printf("anonhash_set opt=%u\n", (unsigned int)opt);
#endif

    if( !SvROK(sv) ){
        warn("assign non-ref value to a hash pattern");
        return 0;
    }

    src = SvRV(sv);
    switch( SvTYPE(src) ){
        case SVt_PVHV:
        case SVt_PVAV:
            break;
        default:
            warn("assign non hash ref value but a ref to a SvTYPE=%d to a hash pattern", SvTYPE(src));
            return 0;
    }

    for(I32 i=0; i<nitems; ++i, ++list_holder){
        if( i==*const_index ){
            key = SvPV(*list_holder, keylen);
#ifdef DEBUG
            printf("got key: %s\n", key);
#endif
            ++const_index;
        }
        else{
            if( SvTYPE(src)==SVt_PVHV ){
                SV ** ptr_val = hv_fetch((HV*)src, key, keylen, 0);
#ifdef DEBUG
                if( ptr_val )
                    printf("got val: %s\n", SvPV_nolen(*ptr_val));
                else
                    printf("got val: NULL\n");
#endif
                my_sv_set(aTHX_ list_holder, ptr_val, (i != -*const_index-1 && opt & OPT_ALIAS));
            }
            else{ /* SvTYPE(src)==SVt_PVAV */
                I32 j = AvFILL((AV*)src);
                if( j>=0 )
                    if( j & 1 )
                        --j;
                    else
                        warn("assign an array with odd number of elements to a hash pattern");

                while( j>=0 ){
                    SV ** target_key_ptr = av_fetch((AV*)src, j, 0);
                    int found;
                    if( target_key_ptr ){
                        STRLEN target_keylen;
                        char * target_key = SvPV(*target_key_ptr, target_keylen);
                        found = (keylen == target_keylen && 0 == memcmp(key, target_key, keylen));
                    }
                    else{
                        found = (keylen == 0);
                    }

                    if( found )
                        break;
                    j -= 2;
                }

                U32 is_alias = (i != -*const_index-1 && opt & OPT_ALIAS);
                if( j>=0 ){ /* found */
                    SV ** target_val_ptr = av_fetch((AV*)src, j+1, (is_alias ? 1 : 0));
                    my_sv_set(aTHX_ list_holder, target_val_ptr, is_alias);
                }
                else{ /* not found */
                    my_sv_set(aTHX_ list_holder, NULL, is_alias);
                }
            }
            if( i == -*const_index-1 )
                ++const_index;
        }
    }
    return 0;
}
static int anonhash_alias_set(pTHX_ SV * sv, MAGIC * mg){
    return anonhash_set_common(aTHX_ sv, mg, OPT_ALIAS);
}
static int anonhash_set(pTHX_ SV * sv, MAGIC * mg){
    return anonhash_set_common(aTHX_ sv, mg, 0);
}

static inline void init_set_vtbl(MGVTBL *vtbl, int(*setter)(pTHX_ SV*, MAGIC*)){
    vtbl->svt_get = NULL;
    vtbl->svt_set = setter;
    vtbl->svt_len = NULL;
    vtbl->svt_clear = NULL;
    vtbl->svt_free = NULL;
}
static MGVTBL anonlist_vtbl, anonlist_alias_vtbl, anonhash_vtbl, anonhash_alias_vtbl;

static inline OP * my_pp_anonlisthash_common(pTHX_ MGVTBL *vtbl){
    dVAR; dSP; dMARK;
    int nitems = SP-MARK;
    I32 holder_size = nitems * sizeof(SV*) + sizeof(I32*);
    char * list_holder = alloca(holder_size);

    Copy(MARK+1, list_holder + sizeof(I32*), nitems, SV*);
    *(I32**)list_holder = (I32*)SvPVX(cSVOPx_sv(OpSIBLING(PL_op)));

    SP = MARK+1;

    SV * ret = SETs(sv_2mortal(newSV(0)));
    SvUPGRADE(ret, SVt_PVMG);
    sv_magicext(ret, ret, PERL_MAGIC_ext, vtbl, list_holder, holder_size);

    RETURN;
}
static OP * my_pp_anonlist(pTHX){
    return my_pp_anonlisthash_common(aTHX_ &anonlist_vtbl);
}
static OP * my_pp_anonlist_alias(pTHX){
    return my_pp_anonlisthash_common(aTHX_ &anonlist_alias_vtbl);
}
static OP * my_pp_anonhash(pTHX){
    return my_pp_anonlisthash_common(aTHX_ &anonhash_vtbl);
}
static OP * my_pp_anonhash_alias(pTHX){
    return my_pp_anonlisthash_common(aTHX_ &anonhash_alias_vtbl);
}
