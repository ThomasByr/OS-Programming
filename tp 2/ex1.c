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

long count;

void *thread_f(void *arg) {
    pthread_mutex_t *mutex = arg;
    for (int i = 0; i < 1000 * 1000; i++) {
        TCHK(pthread_mutex_lock(mutex));
        count++;
        TCHK(pthread_mutex_unlock(mutex));
    }
    return NULL;
}

int main(void) {
    count = 0;
    // launch 4 threads
    pthread_t t[4];
    pthread_mutex_t mutex;
    TCHK(pthread_mutex_init(&mutex, NULL));
    for (int i = 0; i < 4; i++) {
        TCHK(pthread_create(&t[i], NULL, thread_f, &mutex));
    }

    // wait for them to finish
    for (int i = 0; i < 4; i++) {
        TCHK(pthread_join(t[i], NULL));
    }

    // destroy the mutex
    TCHK(pthread_mutex_destroy(&mutex));

    // print count
    printf("count = %ld\n", count);
}
