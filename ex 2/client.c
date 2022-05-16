#include "lib.h"

// negative or null quantity for a product is not allowed
int f(int qty) { return qty > 0; }

/**
 * @brief group redundant products in the list.
 *
 * @param prd list of product names
 * @param qty list of quantities
 * @param n number of products
 * @return int - number of groups
 */
int group_by_prd(char (*prd)[PRD_MAX_LEN + 1], int *qty, int n) {
    int i, j, k;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(prd[i], prd[j]) == 0) {
                qty[i] += qty[j];
                for (k = j; k < n - 1; k++) {
                    strncpy(prd[k], prd[k + 1], PRD_MAX_LEN);
                    qty[k] = qty[k + 1];
                }
                n--;
                j--;
            }
        }
    }
    return n;
}

/**
 * @brief print the list of products and quantities if VERBOSE is set.
 *
 * @param prd product names
 * @param qty quantities
 * @param n number of products
 */
void display_prd(char (*prd)[PRD_MAX_LEN + 1], int *qty, int n) {
    int i;
    debug(1, "list (%d):", n);
    for (i = 0; i < n; i++) {
        debug(0, " (%s: %d)", prd[i], qty[i]);
    }
    debug(0, "\n");
}

/**
 * @brief buy a product from the shop.
 *
 * @param prd product name
 * @param qty quantity to buy
 * @return int - number of products bought
 */
int buy_prd(char prd[PRD_MAX_LEN + 1], int qty) {
    int fd, n, num = 0;

    debug(1, "buying %d product from shop %s\n", qty, prd);

    // named semaphore, created if not existing
    char name[SEM_MAX_LEN + 1];
    set_sem(0, name, prd);

    sem_t *sem;
    named_sem_init(&sem, name, O_CREAT, 0666, 1);

    TCHK(sem_wait(sem)); // wait for possible producer or consumer

    CHK(fd = open(prd, O_RDWR | O_CREAT, 0666));

    struct shop s;
    CHK(n = read(fd, &s, sizeof(s)));

    if (n == 0) {
        debug(0, "\tshop %s has closed\n", prd);
        num = -1;
    }
    if (n > 0 && n != sizeof(s)) {
        panic(0, "file %s corrupted", prd);
    }

    CHK(lseek(fd, 0, SEEK_SET));

    if (n == sizeof(s) && s.qty > 0) {
        if (s.qty >= qty) {
            s.qty -= qty;
            CHK(write(fd, &s, sizeof(s)));
            num = qty;
        } else {
            num = s.qty;
            s.qty = 0;
            CHK(write(fd, &s, sizeof(s)));
        }
        debug(0, "\told_qty = %d -> new_qty = %d\n", s.qty + qty, s.qty);
    }

    CHK(close(fd));

    TCHK(sem_post(sem));  // unlock potential new producer or consumer
    TCHK(sem_close(sem)); // close the semaphore

    return num;
}

struct targ {
    char prd[PRD_MAX_LEN + 1];
    int qty;
};

void *tf(void *arg) {
    struct targ *t = (struct targ *)arg;
    int run = 1;

    debug(1, "thread %lu: buying %d product from shop %s\n", pthread_self(),
          t->qty, t->prd);

    while (run) {
        int num = buy_prd(t->prd, t->qty);

        if (num == -1) {
            run = 0; // the shop is closed
        } else if (num == t->qty) {
            run = 0; // we have bought all we wanted
        } else {
            t->qty -= num;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        panic(0, "usage: %s <prd0> <qty0> <prd1> <qty1> ...", argv[0]);
    }

    /* checking command line arguments */

    int n = argc - 1; // number of couple products, qty
    if (n % 2 != 0) {
        panic(0, "usage: %s <prd0> <qty0> <prd1> <qty1> ...", argv[0]);
    }
    n /= 2; // number of products

    char prd[n][PRD_MAX_LEN + 1]; // product names
    int qty[n];                   // quantities

    for (int i = 0; i < n; i++) {
        set_prd(prd[i], "%s", argv[i * 2 + 1]);
        qty[i] = strtoi(argv[i * 2 + 2], f);
    }

    n = group_by_prd(prd, qty, n); // group redundant products
    display_prd(prd, qty, n);      // display the list

    /* performing the action */

    // launch threads
    // each thread will attempt to buy a product from the shop
    // if the product is not available, the thread will wait
    // until the product becomes available or the shop is closed
    struct targ *t = xmalloc(n * sizeof(struct targ));
    for (int i = 0; i < n; i++) {
        strncpy(t[i].prd, prd[i], PRD_MAX_LEN);
        t[i].qty = qty[i];
    }
    pthread_t *th = xmalloc(n * sizeof(pthread_t));
    for (int i = 0; i < n; i++) {
        TCHK(pthread_create(&th[i], NULL, tf, &t[i]));
    }

    // wait for all threads to finish
    for (int i = 0; i < n; i++) {
        TCHK(pthread_join(th[i], NULL));
    }

    free(t);
    free(th);
    return EXIT_SUCCESS;
}
