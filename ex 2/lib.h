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

#define RST "\x1b[m\x1b[0m"

#define FG_RED "\x1b[0;31m"
#define FG_GRN "\x1b[0;32m"
#define FG_YEL "\x1b[0;33m"
#define FG_BLU "\x1b[0;34m"
#define FG_MAG "\x1b[0;35m"
#define FG_CYN "\x1b[0;36m"
#define FG_WHT "\x1b[0;37m"

#define BG_BLU "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GRN "\x1b[42m"
#define BG_ORA "\x1b[43m"
#define BG_CYN1 "\x1b[44m"
#define BG_YEL "\x1b[45m"
#define BG_CYN2 "\x1b[46m"
#define BG_WHT "\x1b[47m"

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

/**
 * @brief print debug message if VERBOSE is set.
 *
 * @param fmt formated message to print
 * @param first 1 if the first message of a series, 0 otherwise
 */
void debug(int first, const char *restrict fmt, ...);