#include "lib.h"

noreturn void panic(int syserr, const char *restrict fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1) {
        perror("");
    }

    exit(EXIT_FAILURE);
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
        panic(0, "vsnprintf failed");
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
        panic(0, "vsnprintf failed");
    }
    va_end(ap);

    p[PRD_MAX_LEN] = '\0';

    n = snprintf(sem_name, SEM_MAX_LEN + 1, "%s.%d", p, id);
    if (n > SEM_MAX_LEN) {
        panic(0, "semaphore name too long");
    }
    if (n < 0) {
        panic(0, "snprintf failed");
    }

    sem_name[SEM_MAX_LEN] = '\0';
}

void named_sem_init(sem_t **sem, const char *name, int oflags, ...) {
    va_list ap;
    va_start(ap, oflags);

    *sem = sem_open(name, oflags, va_arg(ap, int));
    if (*sem == SEM_FAILED) {
        panic(1, "sem_open failed");
    }
}

void debug(int first, const char *restrict fmt, ...) {
    va_list ap;

    if (VERBOSE) {
        va_start(ap, fmt);
        if (first) {
            fprintf(stdout, FG_GRN "\n  DEBUG: " RST);
        }
        vfprintf(stdout, fmt, ap);
        va_end(ap);
    }
    int n = fflush(stdout);
    if (n == EOF) {
        panic(1, "fflush failed");
    }
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
        panic(1, "fflush failed");
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
