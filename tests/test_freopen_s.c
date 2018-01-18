/*------------------------------------------------------------------
 * test_freopen_s
 * File 'io/freopen_s.c'
 * Lines executed:100.00% of 18
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_lib.h"
#include <unistd.h>

#define TMP "tmpfreopen"

#define HAVE_NATIVE defined(HAVE_FREOPEN_S)
#include "test_msvcrt.h"

int test_freopen_s (void)
{
    errno_t rc;
    int errs = 0;
    FILE *tmp, *newf;
    FILE *file = stdin;

/*--------------------------------------------------*/
    /* shared */
    if (use_msvcrt)
        printf("Using msvcrt...\n");

    rc = freopen_s(NULL, TMP, "r", file);
    if ( rc == ESNULLP ) {
        if (use_msvcrt)
            printf("safec.dll overriding msvcrt.dll\n");
        use_msvcrt = false;
    } else {
        if (!use_msvcrt)
            printf("msvcrt.dll overriding safec.dll\n");
        use_msvcrt = true;
    }
    ERR_MSVC(ESNULLP, EINVAL);

    rc = freopen_s(&tmp, TMP, NULL, file);
    ERR_MSVC(ESNULLP, EINVAL);

    rc = freopen_s(&tmp, TMP, "r", NULL);
    ERR_MSVC(ESNULLP, EINVAL);

/*--------------------------------------------------*/

    rc = freopen_s(&tmp, TMP, "r", file);
    ERR(ENOENT)
    if (errno)
        ERRNO(ENOENT);

/*--------------------------------------------------*/

    /* TODO: fails with asan and valgrind on some glibc systems (not repro)
       in strlen or __open_nocancel. glibc bug. */
#ifndef __GLIBC__
# ifndef HAVE_ASAN
    file = stdin;
    rc = freopen_s(&tmp, NULL, "rb", file);
    if (rc == 0) {
        ERR(0); /* EINVAL or EFAULT */
        if (errno)
            ERRNO(EINVAL);
    }
# endif
#endif

/*--------------------------------------------------*/

    tmp = fopen(TMP, "w");
    if (tmp == NULL) {
        puts ("fopen failed: %m");
        return errs+1;
    }

    fputs("Hello 1\n", tmp);
    rc = freopen_s(&newf, TMP, "a+", tmp);
    ERR(0);
    fputs("Hello 2\n", newf);
    fclose(newf);

/*--------------------------------------------------*/

    unlink(TMP);

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_freopen_s());
}
#endif
