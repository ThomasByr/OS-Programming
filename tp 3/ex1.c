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

unsigned int nbMachines;

void *job(void *arg) {
    unsigned int *t = arg;
    sleep(*t);
    printf("Job done\n");
    fflush(stdout);
    return NULL;
}

struct thread_arg {
    unsigned int p;
    unsigned int m;
    pthread_cond_t *cond;
    pthread_mutex_t *mutex;
    void *(*function)(void *);
};

void *thread_function(void *arg) {
    // Get arguments
    struct thread_arg *targ = arg;
    unsigned int p = targ->p;
    unsigned int m = targ->m;
    pthread_cond_t *cond = targ->cond;
    pthread_mutex_t *mutex = targ->mutex;
    void *(*function)(void *) = targ->function;

    // Get number of machines for each jobs
    unsigned int *k = malloc(sizeof(unsigned int) * p);
    if (k == NULL) {
        panic(0, "malloc");
    }
    k[0] = 1 + ((unsigned int)pthread_self() % m);
    for (unsigned int i = 1; i < p; i++) {
        k[i] = 1 + (rand_r(&k[i - 1]) % m);
        k[i - 1] = 1 + (k[i - 1] % m);
    }
    // Get time for each sleep
    unsigned int *t = malloc(sizeof(unsigned int) * p);
    if (t == NULL) {
        panic(0, "malloc");
    }
    t[0] = 1 + ((unsigned int)pthread_self() % 3);
    for (unsigned int i = 1; i < p; i++) {
        t[i] = rand_r(&t[i - 1]) % 3;
        t[i - 1] = 1 + (t[i - 1] % 3);
    }

    // Launch jobs
    for (unsigned int i = 0; i < p; i++) {
        // Wait for k[i] machines available
        TCHK(pthread_mutex_lock(mutex));
        while (nbMachines < k[i]) {
            printf("Job %u: is waiting for %u machines, on thread %lu\n", i,
                   k[i], pthread_self());
            fflush(stdout);
            TCHK(pthread_cond_wait(cond, mutex));
        }
        TCHK(pthread_mutex_unlock(mutex));
        TCHK(pthread_mutex_lock(mutex));
        printf("Job %u: use %u machines available, on thread %lu\n", i, k[i],
               pthread_self());
        nbMachines -= k[i];
        TCHK(pthread_mutex_unlock(mutex));
        function(&t[i]);
        TCHK(pthread_mutex_lock(mutex));
        nbMachines += k[i];
        TCHK(pthread_mutex_unlock(mutex));
        TCHK(pthread_cond_broadcast(cond));
    }

    free(k);
    free(t);
    return NULL;
}

int main(int argc, char **argv) {
    // Check arguments
    if (argc != 4) {
        panic(0, "Usage: %s <m> <n> <p>", argv[0]);
    }

    // Get arguments
    unsigned int m = atoi(argv[1]);
    nbMachines = m;
    unsigned int n = atoi(argv[2]);
    unsigned int p = atoi(argv[3]);

    // Initialize threads
    pthread_t *threads = malloc(sizeof(pthread_t) * n);

    // Initialize conditions
    pthread_cond_t cond;
    TCHK(pthread_cond_init(&cond, NULL));
    pthread_mutex_t mutex;
    TCHK(pthread_mutex_init(&mutex, NULL));

    // Initialize arguments
    struct thread_arg arg = {p, m, &cond, &mutex, job};

    // Launch threads
    for (unsigned int i = 0; i < n; i++) {
        TCHK(pthread_create(&threads[i], NULL, thread_function, &arg));
    }

    // Wait for threads
    for (unsigned int i = 0; i < n; i++) {
        TCHK(pthread_join(threads[i], NULL));
    }

    free(threads);
    TCHK(pthread_cond_destroy(&cond));
    TCHK(pthread_mutex_destroy(&mutex));
    exit(EXIT_SUCCESS);
}
