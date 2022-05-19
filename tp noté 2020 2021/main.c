#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <unistd.h>

#define MAXLEN 1000 // longueur maximum d'une ligne

#define NCHK(op)            \
    do {                    \
        if ((op) == NULL) { \
            panic(1, #op);  \
        }                   \
    } while (0)
#define TCHK(op)                  \
    do {                          \
        if ((errno = (op)) > 0) { \
            panic(1, #op);        \
        }                         \
    } while (0)

struct arg {
    char *fichier;      // fichier que le thread doit lire
    char ligne[MAXLEN]; // chaque ligne lue par le thread
    int *termine;       // indique si le thread est terminé
    sem_t *sem_r;       // sémaphore de lecture
    sem_t *sem_w;       // sémaphore d'écriture
};

noreturn void panic(int syserr, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr) {
        perror("");
    }

    exit(EXIT_FAILURE);
}

void *f(void *arg) {
    struct arg *a = arg;
    char ligne[MAXLEN]; // y compris le \0 de fin de chaine
    FILE *fp;

    NCHK(fp = fopen(a->fichier, "r"));
    // lire la ligne dans la variable locale du thread, pendant que le thread
    // principal affiche la ligne partagée

    while (fgets(ligne, sizeof ligne, fp) != NULL) {
        // recopier la ligne lue dans la variable partagée
        TCHK(sem_wait(a->sem_r));
        strcpy(a->ligne, ligne);
        TCHK(sem_post(a->sem_w));
    }

    if (fclose(fp) == EOF) {
        panic(1, "fclose %s", a->fichier);
    }

    TCHK(sem_wait(a->sem_r));
    a->ligne[0] = '\0';
    *a->termine = 1;
    TCHK(sem_post(a->sem_w));

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t id;
    struct arg arg;
    int termine = 0;

    sem_t sem_r;
    sem_t sem_w;
    TCHK(sem_init(&sem_r, 0, 1));
    TCHK(sem_init(&sem_w, 0, 0));

    if (argc != 2) {
        panic(0, "usage: %s <file>", argv[0]);
    }

    arg.fichier = argv[1];
    arg.sem_r = &sem_r;
    arg.sem_w = &sem_w;

    arg.termine = &termine;

    TCHK(pthread_create(&id, NULL, f, &arg));

    while (!termine) {
        TCHK(sem_wait(&sem_w));
        printf("%s", arg.ligne);
        fflush(stdout);
        TCHK(sem_post(&sem_r));
    }

    TCHK(pthread_join(id, NULL));

    TCHK(sem_destroy(&sem_r));
    TCHK(sem_destroy(&sem_w));

    exit(EXIT_SUCCESS);
}
