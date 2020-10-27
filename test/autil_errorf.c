#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    autil_errorf("SOME ERROR MESSAGE");
    puts("THIS SHOULD GET PRINTED");
    return EXIT_SUCCESS;
}
