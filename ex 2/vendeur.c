#include "lib.h"

// strict negative quantity for a product is not allowed
int f(int qty) { return qty >= 0; }

/**
 * @brief closes a shop by removing contents from the stock.
 *
 * @param prd product name
 */
void close_shop(char *prd) {
    int fd;
    debug(1, "closing shop for %s\n", prd);
    CHK(fd = open(prd, O_RDWR | O_TRUNC));
    CHK(close(fd));
}

/**
 * @brief add a product to the stock by either adding or creating it.
 *
 * @param prd product name
 * @param qty quantity to add or create
 */
void add_to_shop(char *prd, int qty) {
    int fd, old_qty, n;
    CHK(fd = open(prd, O_RDWR | O_CREAT, 0666));

    debug(1, "adding %d product to shop %s\n", qty, prd);

    // is the file empty?
    if ((n = read(fd, &old_qty, sizeof(old_qty))) != sizeof(old_qty)) {
        old_qty = 0;
    }
    if (n == -1) {
        panic(1, "read");
    }

    debug(0, "\told_qty = %d -> new_qty = %d\n", old_qty, old_qty + qty);

    // update the file
    old_qty += qty;
    CHK(lseek(fd, 0, SEEK_SET));
    CHK(write(fd, &old_qty, sizeof(old_qty)));

    CHK(close(fd));
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
