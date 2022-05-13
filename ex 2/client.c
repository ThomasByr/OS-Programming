#include "lib.h"

// strict negative quantity for a product is not allowed
int f(int qty) { return qty >= 0; }

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
                    strcpy(prd[k], prd[k + 1]);
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
 * @brief prints the list of products and quantities if VERBOSE is set.
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

    // todo: implement
}
