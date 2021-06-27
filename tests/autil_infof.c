#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    autil_infof("SOME INFO MESSAGE");
    puts("THIS SHOULD GET PRINTED");
    return EXIT_SUCCESS;
}
