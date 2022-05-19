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
 * @brief print the list of products and quantities if DEBUG is set.
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

    CHK(fd = open(prd, O_RDWR, 0666));

    debug(1, "buying %d product from shop %s\n", qty, prd);

    // named semaphore, created if not existing
    char sem_name[SEM_MAX_LEN + 1], cnd_name[SEM_MAX_LEN + 1];
    set_sem(0, sem_name, prd);
    set_sem(1, cnd_name, prd);

    sem_t *sem, *cnd; // file protection, condition

    // these should be created when the shop is opened
    // if the shop does not exist, we should panic anyway
    named_sem_init(&sem, sem_name, 0, 0666);
    named_sem_init(&cnd, cnd_name, 0, 0666);

    TCHK(sem_wait(cnd)); // wait for the condition
    TCHK(sem_wait(sem)); // wait for possible producer or consumer

    // fd = open(prd, O_RDWR, 0666);
    // switch (fd) {
    // case -1:
    //     debug(0, "\tshop %s has closed\n", prd);
    //     TCHK(sem_post(sem));
    //     TCHK(sem_post(cnd)); // allow the next consumer to panic

    //     CHK(sem_close(sem)); // close the semaphore
    //     CHK(sem_close(cnd));
    //     return -1;
    // }

    struct shop s;
    shop_init(&s);
    CHK(n = read(fd, &s, sizeof(s)));

    if (n == 0) {
        debug(0, "\tshop %s has closed\n", prd);
        TCHK(sem_post(sem)); // allow the next consumer to panic
        TCHK(sem_post(cnd));
        num = -1;
    }
    if (n != sizeof(s)) {
        panic(0, "file %s corrupted", prd);
    }

    CHK(lseek(fd, 0, SEEK_SET)); // rewind

    if (n == sizeof(s) && s.qty > 0) {
        if (s.qty >= qty) {
            s.qty -= qty;
            num = qty;
        } else {
            num = s.qty;
            s.qty = 0;
        }
        CHK(write(fd, &s, sizeof(s)));
        debug(0, "\told_qty = %d -> new_qty = %d\n", s.qty + num, s.qty);
    }

    CHK(close(fd));

    TCHK(sem_post(sem)); // unlock potential new producer or consumer
    if (s.qty > 0) {
        TCHK(sem_post(cnd)); // signal the condition if shop is not empty
    }

    CHK(sem_close(sem)); // close the semaphore
    CHK(sem_close(cnd));

    return num;
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

    // for each product
    // if the shop is closed, quit
    // if the shop is open, buy the desired quantity if possible
    // if the product is not available, wait until it is available
    int i, _qty, num;
    for (i = 0; i < n; i++) {
        _qty = qty[i];

        while (_qty > 0) {
            num = buy_prd(prd[i], _qty);
            if (num < 0) {
                panic(0, "shop %s closed", prd[i]);
            }
            _qty -= num;
        }
    }

    return EXIT_SUCCESS;
}
