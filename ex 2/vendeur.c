#include "lib.h"

// strict negative quantity for a product is not allowed
int f(int qty) { return qty >= 0; }

/**
 * @brief closes a shop by removing contents from the stock.
 *
 * @param prd product name
 */
void close_shop(char prd[PRD_MAX_LEN + 1]) {
    int fd;
    debug(1, "closing shop for %s\n", prd);

    // named semaphore, created if not existing
    char sell[SEM_MAX_LEN + 1], buy[SEM_MAX_LEN + 1];
    set_sem(0, sell, "%s", prd);
    set_sem(1, buy, "%s", prd);

    sem_t *sem_sell, *sem_buy;
    named_sem_init(&sem_sell, sell, O_CREAT, 0666, 1);
    named_sem_init(&sem_buy, buy, O_CREAT, 0666, 1);

    TCHK(sem_wait(sem_sell)); // wait for possible producer
    TCHK(sem_wait(sem_buy));  // wait for possible consumer

    CHK(fd = open(prd, O_RDWR | O_TRUNC | O_CREAT, 0666)); // delete contents
    CHK(close(fd));                                        // close the file

    TCHK(sem_post(sem_sell)); // unlock potential new producer
    TCHK(sem_post(sem_buy));  // unlock potential buyers
                              //(most certainly will they be angry)

    TCHK(sem_close(sem_sell)); // close the semaphore
    TCHK(sem_close(sem_buy));  // close the semaphore
}

/**
 * @brief add a product to the stock by either adding or creating it.
 *
 * @param prd product name
 * @param qty quantity to add or create
 */
void add_to_shop(char prd[PRD_MAX_LEN + 1], int qty) {
    int fd, n;
    struct shop s;
    // named semaphore, created if not existing
    char sell[SEM_MAX_LEN + 1], buy[SEM_MAX_LEN + 1];
    set_sem(0, sell, "%s", prd);
    set_sem(1, buy, "%s", prd);

    sem_t *sem_sell, *sem_buy;
    named_sem_init(&sem_sell, sell, O_CREAT, 0666, 1);
    named_sem_init(&sem_buy, buy, O_CREAT, 0666, 1);

    TCHK(sem_wait(sem_sell)); // wait for possible producer
    TCHK(sem_wait(sem_buy));  // wait for possible consumer

    CHK(fd = open(prd, O_RDWR | O_CREAT, 0666));

    debug(1, "adding %d product to shop %s\n", qty, prd);

    if ((n = read(fd, &s, sizeof(s))) != sizeof(s)) {
        s.qty = 0;
    } // is the file empty?
    if (n == -1) {
        panic(1, "read");
    } // error reading file

    debug(0, "\told_qty = %d -> new_qty = %d\n", s.qty, s.qty + qty);

    // update the file
    s.qty += qty;
    CHK(lseek(fd, 0, SEEK_SET));
    CHK(write(fd, &s, sizeof(s)));

    CHK(close(fd));

    TCHK(sem_post(sem_sell)); // unlock potential new producer
    TCHK(sem_post(sem_buy));  // unlock potential buyers

    TCHK(sem_close(sem_sell)); // close the semaphore
    TCHK(sem_close(sem_buy));  // close the semaphore
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        panic(1, "usage: %s <prd> <qty>", argv[0]);
    }

    /* checking command line arguments */

    char prd[PRD_MAX_LEN + 1];
    set_prd(prd, "%s", argv[1]);

    int qty = strtoi(argv[2], f);

    /* performing the action */

    switch (qty) {
    case 0:
        close_shop(prd);
        break;
    default:
        add_to_shop(prd, qty);
        break;
    }

    return EXIT_SUCCESS;
}
