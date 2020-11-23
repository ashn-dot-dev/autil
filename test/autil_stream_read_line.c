#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#include "test.h"

void* buf = NULL;
size_t size = 0;

FILE* fp = NULL;
int err = 0;

static void
text_file(void)
{
    fp = fopen("data/test.txt", "rb");
    ASSERT(fp != NULL);

    err =  autil_stream_read_line(fp, &buf, &size);
    ASSERT(err == 0);
    printf("1: %.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);

    err =  autil_stream_read_line(fp, &buf, &size);
    ASSERT(err == 0);
    printf("2: %.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);

    err =  autil_stream_read_line(fp, &buf, &size);
    ASSERT(err == 0);
    printf("3: %.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);

    fclose(fp);
}

static void
text_file_no_trailing_newline(void)
{
    fp = fopen("data/test-no-trailing-newline.txt", "rb");
    ASSERT(fp != NULL);

    err =  autil_stream_read_line(fp, &buf, &size);
    ASSERT(err == 0);
    printf("1: %.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);

    err =  autil_stream_read_line(fp, &buf, &size);
    ASSERT(err == 0);
    printf("2: %.*s", (int)size, (char*)buf);
    autil_xalloc(buf, AUTIL_XALLOC_FREE);
    fputc('\n', stdout); // Add trailing newline manually

    fclose(fp);
}

int
main(void)
{
    text_file();
    text_file_no_trailing_newline();

    return EXIT_SUCCESS;
}

