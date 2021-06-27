#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    autil_fatalf("SOME ERROR MESSAGE");
    puts("THIS SHOULD NOT GET PRINTED");
    return EXIT_SUCCESS;
}
