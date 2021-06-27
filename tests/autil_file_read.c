#define AUTIL_IMPLEMENTATION
#include "../autil.h"

int
main(void)
{
    void* buf = NULL;
    size_t size = 0;

    if (0 != autil_file_read("data/test.txt", &buf, &size)) {
        fprintf(stderr, "REPORTED FAILURE READING FILE THAT EXISTS");
    }
    printf("%.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);

    if (0 == autil_file_read("file/that/does/not/exist.txt", &buf, &size)) {
        fprintf(stderr, "REPORTED SUCCESS READING FILE THAT DOES NOT EXIST");
    }

    return EXIT_SUCCESS;
}
