#include "string.h"
#include "fcb.h"

FCB foo2[NFCB];
FCB *fcbs = foo2;
static FCB *fcb_free_list;

void fcb_init()
{
	int i;
	memset((void *)fcbs, 0, NFCB * sizeof(FCB));
	for (i = NFCB - 1; i >= 0; i --) {
		fcbs[i].fcb_link = fcb_free_list;
		fcb_free_list = fcbs + i;
	}
}

