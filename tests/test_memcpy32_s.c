/*------------------------------------------------------------------
 * test_memcpy32_s
 * File 'extmem/memcpy32_s.c'
 * Lines executed:86.36% of 22
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "test_expmem.h"

#define LEN   ( 1024 )
#define MAX   ( LEN * 4 )

int main(void)
{
    errno_t rc;
    uint32_t len;
    uint32_t i;

    uint32_t  mem1[LEN];
    uint32_t  mem2[LEN];
    rsize_t count = LEN;
    int errs = 0;

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }

    rc = memcpy32_s(NULL, MAX, mem2, count);
    ERR(ESNULLP);
/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, 0, mem2, count);
    ERR(ESZEROL); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    rc = memcpy32_s(mem1, RSIZE_MAX_MEM+1, mem2, count);
    ERR(ESLEMAX); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);
#endif

/*--------------------------------------------------*/

    rc = memcpy32_s(mem1, MAX, NULL, count);
    ERR(ESNULLP); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }
    
    rc = memcpy32_s(mem1, 12, mem2, 0);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 33, 4);

/*--------------------------------------------------*/

#ifndef HAVE_CT_BOS_OVR
    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    EXPECT_BOS("src overflow or empty") EXPECT_BOS("slen overflow >dmax/4")
    rc = memcpy32_s(mem1, MAX, mem2, RSIZE_MAX_MEM32+1);
    ERR(ESLEMAX); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);
#endif
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 1;
    rc = memcpy32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN-len, 33, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    len = 2;
    rc = memcpy32_s(mem1, MAX, mem2, len);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, len, 44, 4);
    EXPMEM(mem1, len, LEN-len, 33, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 33; }
    for (i=0; i<LEN; i++) { mem2[i] = 44; }

    /* count*4 greater than dmax */
    rc = memcpy32_s(mem1, MAX, mem2, count+1);
    ERR(ESNOSPC); /* and cleared */
    EXPMEM(mem1, 0, LEN, 0, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=0; i<LEN; i++) { mem2[i] = 35; }

    /* same ptr */
    rc = memcpy32_s(mem1, MAX, mem1, count);
    ERR(EOK); /* and untouched */
    EXPMEM(mem1, 0, LEN, 25, 4);
/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 25; }
    for (i=0; i<LEN; i++) { mem2[i] = 35; }

    /* overlap + */
    rc = memcpy32_s(&mem1[0], LEN, &mem1[10], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 0, 100, 0, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 65; }
    for (i=0; i<LEN; i++) { mem2[i] = 75; }

    /* overlap - */
    rc = memcpy32_s(&mem1[10], LEN-10, &mem1[0], 100);
    ERR(ESOVRLP); /* and cleared */
    EXPMEM(mem1, 10, LEN-10, 0, 4);
    EXPMEM(mem1, 0, 10, 65, 4);

/*--------------------------------------------------*/

    for (i=0; i<LEN; i++) { mem1[i] = 65; }
    for (i=0; i<LEN; i++) { mem2[i] = 75; }

    rc = memcpy32_s(mem1, MAX, mem2, count-10);
    ERR(EOK); /* and copied */
    EXPMEM(mem1, 0, count-10, 75, 4);
    EXPMEM(mem1, count-10, LEN, 65, 4);

/*--------------------------------------------------*/

    return (errs);
}
