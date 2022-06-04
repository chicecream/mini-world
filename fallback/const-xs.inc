#include "ppport.h"
void
constant(sv)
    PREINIT:
#ifdef dXSTARG
	dXSTARG; /* Faster if we have it.  */
#else
	dTARGET;
#endif
	STRLEN		len;
        int		type;
	/* IV		iv;	Uncomment this if you need to return IVs */
	/* NV		nv;	Unc