#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rng.h"


int main()
{
    rng_init(123123);
    printf("%d ", rng_rand(0, 100));
    printf("%d ", rng_rand(0, 100));
    printf("%d\n", rng_rand(0, 100));

    // Results should be the same after reinitialization
    rng_init(123123);
    printf("%d ", rng_rand(0, 100));
    printf("%d ", rng_rand(0, 100));
    printf("%d\n", rng_rand(0, 100));

    return 0;
}
