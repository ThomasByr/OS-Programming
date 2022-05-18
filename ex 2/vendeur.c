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
    char sem_name[SEM_MAX_LEN + 1], cnd_name[SEM_MAX_LEN + 1];
    set_sem(0, sem_name, prd);
    set_sem(1, cnd_name, prd);

    sem_t *sem, *cnd; // file protection, condition
    named_sem_init(&sem, sem_name, O_CREAT, 0666, 1);
    named_sem_init(&cnd, cnd_name, O_CREAT, 0666, 0);

    TCHK(sem_wait(sem)); // wait for possible producer or customer

    CHK(fd = open(prd, O_RDWR | O_TRUNC, 0666)); // delete contents
    CHK(close(fd));                              // close the file
    CHK(unlink(prd));                            // remove the file

    TCHK(sem_post(sem)); // unlock potential new producer or customer
    TCHK(sem_post(cnd)); // signal the condition

    TCHK(sem_close(sem)); // close the semaphore
    TCHK(sem_close(cnd));

    CHK(sem_unlink(sem_name)); // remove the semaphore
    CHK(sem_unlink(cnd_name));

    debug(0, "\t%s closed\n", prd);
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
    char sem_name[SEM_MAX_LEN + 1], cnd_name[SEM_MAX_LEN + 1];
    set_sem(0, sem_name, prd);
    set_sem(1, cnd_name, prd);

    sem_t *sem, *cnd; // file protection, condition
    named_sem_init(&sem, sem_name, O_CREAT, 0666, 1);
    named_sem_init(&cnd, cnd_name, O_CREAT, 0666, 0);

    TCHK(sem_wait(sem)); // wait for possible producer or consumer

    CHK(fd = open(prd, O_RDWR | O_CREAT, 0666));

    debug(1, "adding %d product to shop %s\n", qty, prd);

    if ((n = read(fd, &s, sizeof(s))) == 0) {
        s.qty = 0;
    } // is the file empty?
    if (n > 0 && n != sizeof(s)) {
        panic(0, "file %s corrupted", prd);
    } // we did not read the whole structure properly
    if (n == -1) {
        panic(1, "read");
    } // error reading file

    debug(0, "\told_qty = %d -> new_qty = %d\n", s.qty, s.qty + qty);

    // update the file
    s.qty += qty;
    CHK(lseek(fd, 0, SEEK_SET));
    CHK(write(fd, &s, sizeof(s)));

    CHK(close(fd));

    TCHK(sem_post(sem)); // unlock potential new producer or consumer
    TCHK(sem_post(cnd)); // signal the condition

    TCHK(sem_close(sem)); // close the semaphore
    TCHK(sem_close(cnd));

    debug(0, "\t%s updated\n", prd);
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
