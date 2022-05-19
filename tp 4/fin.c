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
#define ONE 1

noreturn void panic(int syserr, const char *msg, ...) {
    va_list ap;

    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1)
        perror("");

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        panic(0, "usage: %s <filename>", argv[0]);
    }

    int fd;
    char *tab;
    CHK(fd = open(argv[1], O_RDONLY));

    struct stat s;
    CHK(fstat(fd, &s));

    tab = mmap(NULL, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (tab == MAP_FAILED) {
        panic(1, "mmap");
    }

    CHK(close(fd));

    char *p = tab + s.st_size - ONE;
    int i = 0;
    while (p > tab && *p != '\n') {
        i++;
        p--;
    }

    if (p >= tab) {
        if (*p == '\n') {
            p++;
        }
        CHK(write(STDOUT_FILENO, p, i));
    }

    CHK(munmap(tab, s.st_size));
}
// help me
// coucou