#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>

#define CHK(op)            \
    do {                   \
        if ((op) == -1)    \
            panic(1, #op); \
    } while (0)

#define TCHK(op)                \
    do {                        \
        if ((errno = (op)) > 0) \
            panic(1, #op);      \
    } while (0)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define VERBOSE 1      // 0 or 1
#define PRD_MAX_LEN 10 // max length of product name

/**
 * @brief panic with a message.
 *
 * @param syserr 1 if errno is set, 0 otherwise
 * @param fmt formated message to print
 * @note this function never returns
 */
noreturn void panic(int syserr, const char *restrict fmt, ...);

/**
 * @brief perform a string to int conversion.
 *
 * @param nptr string to convert
 * @param f function to perform additional checks (should return 0 if not legal)
 * @return int - the converted value
 */
int strtoi(const char *restrict nptr, int (*f)(int));

/**
 * @brief set a product name from command line arguments.
 *
 * @param prd product name to set
 * @param fmt suitable format string (max `PRD_MAX_LEN` characters)
 */
void set_prd(char *restrict prd, const char *restrict fmt, ...);
