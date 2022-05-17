#include "lib.h"

noreturn void panic(int syserr, const char *restrict fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, FG_RED "\n  ERROR: " RST);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1) {
        perror("\t");
    }

    exit(EXIT_FAILURE);
}

void snprintf_s(char *restrict str, size_t size, const char *restrict fmt,
                ...) {
    int n;
    va_list ap;

    va_start(ap, fmt);
    n = vsnprintf(str, size, fmt, ap);
    va_end(ap);

    if (n < 0) {
        panic(1, "vsnprintf failure");
    }
    if ((size_t)n >= size) {
        panic(1, "format string too long");
    }
}

int strtoi(const char *restrict nptr, int (*f)(int)) {
    char *endptr;
    int x = strtol(nptr, &endptr, 10);

    if (endptr == nptr || *endptr != '\0') {
        panic(1, "must be a number");
    } // if not a number
    if (errno == ERANGE) {
        panic(1, "out of range [%ld, %ld]", LONG_MIN, LONG_MAX);
    } // if out of range of long (might be a problem with implicit cast)
    if (f != NULL && !f(x)) {
        panic(0, "non legal value for our program");
    } // if non legal value for our program

    return x;
}

void set_prd(char *restrict prd, const char *restrict fmt, ...) {
    int n;
    va_list ap;

    va_start(ap, fmt);
    if ((n = vsnprintf(prd, PRD_MAX_LEN + 1, fmt, ap)) > PRD_MAX_LEN) {
        panic(0, "product name too long");
    }
    if (n < 0) {
        panic(0, "vsnprintf failure");
    }
    va_end(ap);

    prd[PRD_MAX_LEN] = '\0';
}

void set_sem(int id, char *restrict sem_name, char *restrict fmt, ...) {
    int n;
    char p[PRD_MAX_LEN + 1];
    va_list ap;

    va_start(ap, fmt);
    if ((n = vsnprintf(p, PRD_MAX_LEN + 1, fmt, ap)) > PRD_MAX_LEN) {
        panic(0, "product name too long");
    }
    if (n < 0) {
        panic(0, "vsnprintf failure");
    }
    va_end(ap);

    p[PRD_MAX_LEN] = '\0';

    snprintf_s(sem_name, SEM_MAX_LEN + 1, "%s.%d", p, id);

    sem_name[SEM_MAX_LEN] = '\0';
}

void named_sem_init(sem_t **sem, const char *name, int oflags, ...) {
    va_list ap;
    va_start(ap, oflags);

    *sem = sem_open(name, oflags, va_arg(ap, int));
    if (*sem == SEM_FAILED) {
        panic(1, "sem_open failure");
    }
}

void debug(int first, const char *restrict fmt, ...) {

#ifndef DEBUG
    (void)first;
    (void)fmt;
#else
    va_list ap;

    va_start(ap, fmt);
    if (first) {
        fprintf(stdout, FG_GRN "\n  DEBUG: " RST);
    }
    vfprintf(stdout, fmt, ap);
    va_end(ap);

    int n = fflush(stdout);
    if (n == EOF) {
        panic(1, "fflush failure");
    }
#endif
}

void info(int first, const char *restrict fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    if (first) {
        fprintf(stdout, FG_BLU "\n   INFO: " RST);
    }
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    int n = fflush(stdout);
    if (n == EOF) {
        panic(1, "fflush failure");
    }
}

void *xmalloc(size_t size) {
    // void *p = sbrk(0);
    // void *request = sbrk(size);
    // if (request == (void *)-1) {
    //     panic(1, "sbrk failure");
    // }
    // assert(p == request); // Not thread safe.
    // return p;

    void *p = malloc(size);
    if (p == NULL) {
        panic(1, "malloc failure");
    }
    return p;
}
