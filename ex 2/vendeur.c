#include "lib.h"

// strict negative quantity for a product is not allowed
int f(int qty) { return qty >= 0; }

// todo: implement
void close_shop(char *prd) { (void)prd; }

// todo: implement
void add_to_shop(char *prd, int qty) {
    (void)prd;
    (void)qty;
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
