#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
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

struct thread_info {
    pthread_t id;
    pthread_barrier_t *bar;
    int i, p;
    char *c;
};

void *func(void *args) {
    struct thread_info *info = (struct thread_info *)args;

    int i = info->i;
    char *c = info->c;

    for (int k = 0; k < info->p; k++) {
        TCHK(pthread_barrier_wait(info->bar));
        c[i] = k % 2 ? '-' : '#';
        if ((errno = pthread_barrier_wait(info->bar)) > 0) {
            panic(1, "pthread_barrier_wait");
        } else if (errno == PTHREAD_BARRIER_SERIAL_THREAD) {
            printf("%s\n", c);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        panic(0, "usage: %s <n> <p>", argv[0]);
    }

    int n = atoi(argv[1]), p = atoi(argv[2]);
    if (n <= 0 || p <= 0) {
        panic(0, "n and p must be positive");
    }

    struct thread_info *info = calloc(n, sizeof(struct thread_info));
    char *c = calloc(n + 1, sizeof(char));
    if (info == NULL || c == NULL) {
        panic(1, "calloc on info or c");
    }
    c[n] = '\0';

    int i;
    pthread_barrier_t bar;
    CHK(pthread_barrier_init(&bar, NULL, n));
    for (i = 0; i < n; i++) {
        info[i].bar = &bar; // barrier
        info[i].i = i;      // write index
        info[i].p = p;      // max time to write
        info[i].c = c;      // where to write
        if ((errno = pthread_create(&info[i].id, NULL, func, &info[i])) > 0) {
            panic(1, "pthread_create");
        }
    }

    for (i = 0; i < n; i++) {
        TCHK(pthread_join(info[i].id, NULL));
    }
    CHK(pthread_barrier_destroy(&bar));

    free(info);
    free(c);
    return 0;
}
