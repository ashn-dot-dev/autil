#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include <string.h>

int
main(void)
{
    char const* const buf = "THIS IS A TEST STRING\n";
    size_t const size = strlen(buf);

    if (0 != autil_file_write("file-write-target", buf, size)) {
        fprintf(stderr, "REPORTED FAILURE WRITING FILE THAT EXISTS");
    }

    return EXIT_SUCCESS;
}
