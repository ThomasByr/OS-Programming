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
    pthread_t thread_id;
    pthread_barrier_t *bar;
    int p, t;
};

void *func(void *argc) {
    struct thread_info *info = (struct thread_info *)argc;
    int p = info->p, t = info->t;

    int *u = calloc(1, sizeof(int));
    if (u == NULL) {
        panic(1, "calloc on u");
    }
    CHK(pthread_barrier_wait(info->bar));
    for (int i = 1; i <= p; i++) {
        *u += (t - 1) * p + i;
    }
    return u;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        panic(0, "usage: %s <p> <t>", argv[0]);
    }

    int n = atoi(argv[1]);
    int p = atoi(argv[2]);
    int t;

    if (n <= 0) {
        panic(0, "n must be positive");
    }

    struct thread_info *args = calloc(n, sizeof(struct thread_info));
    pthread_t *ids = calloc(n, sizeof(pthread_t));
    if (args == NULL) {
        panic(1, "calloc on args");
    }
    if (ids == NULL) {
        free(args);
        panic(1, "calloc on ids");
    }

    int i;
    pthread_barrier_t bar;
    CHK(pthread_barrier_init(&bar, NULL, n));
    for (i = 0; i < n; i++) {
        t = i + 1;
        args[i].thread_id = ids[i];
        args[i].p = p;
        args[i].t = t;
        args[i].bar = &bar;
        if ((errno = pthread_create(&ids[i], NULL, func, &args[i])) > 0) {
            panic(1, "pthread_create");
        }
    }

    int *u, sum = 0;
    for (i = 0; i < n; i++) {
        if ((errno = pthread_join(ids[i], (void **)&u)) > 0) {
            panic(1, "pthread_join");
        }
        sum += *u;
        printf("%d\n", *u);
        free(u);
    }

    free(args);
    free(ids);

    CHK(pthread_barrier_destroy(&bar));
    printf("sum :%d\n", sum);
    return 0;
}
