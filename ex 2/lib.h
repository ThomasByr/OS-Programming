#pragma once

#include <assert.h>
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

// #define DEBUG          // comment this line to disable debug messages
#define PRD_MAX_LEN 10 // max length of product name
#define SEM_DFF_LEN 3  // max length of semaphore id

#define SEM_MAX_LEN PRD_MAX_LEN + SEM_DFF_LEN

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
 * @brief structure to handle the content of a shop.
 *
 */
struct shop {
    int qty; // quantity of the product
    int nwc; // number of waiting consumers
};

/**
 * @brief panic with a message.
 *
 * @param syserr 1 if errno is set, 0 otherwise
 * @param fmt formated message to print
 * @note this function never returns
 */
noreturn void panic(int syserr, const char *restrict fmt, ...);

/**
 * @brief safe call to snprintf.
 *
 * @param str string to write to
 * @param size size of the string
 * @param fmt formated message to print
 */
void snprintf_s(char *restrict str, size_t size, const char *restrict fmt, ...);

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
 * @brief set a semaphore name from a product name.
 *
 * @param id suffixe id (max `SEM_DFF_LEN` characters)
 * @param sem_name semaphore name to set
 * @param fmt formated product name (max `PRD_MAX_LEN` characters)
 */
void set_sem(int id, char *restrict sem_name, char *restrict fmt, ...);

/**
 * @brief initialize a named semaphore.
 *
 * @param sem semaphore to initialize
 * @param name name of the semaphore
 * @param oflags open flags
 */
void named_sem_init(sem_t **sem, const char *name, int oflags, ...);

/**
 * @brief print debug message if DEBUG is set.
 *
 * @param first 1 if the first message of a series, 0 otherwise
 * @param fmt formated message to print
 */
void debug(int first, const char *restrict fmt, ...);

/**
 * @brief print an info message.
 *
 * @param first 1 if the first message of a series, 0 otherwise
 * @param fmt formated message to print
 */
void info(int first, const char *restrict fmt, ...);

/**
 * @brief allocates memory and panics if it fails.
 *
 * @param size size of the memory to allocate
 * @return void* - pointer to the allocated memory
 */
void *xmalloc(size_t size);

/**
 * @brief init shop structure.
 *
 * @param s shop structure
 */
void shop_init(struct shop *s);
