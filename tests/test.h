#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#define EXPECT(condition)                                                      \
    do {                                                                       \
        if (!(condition))                                                      \
            autil_errorf(                                                      \
                "[%s:%d][%s] EXPECT FAILURE: %s",                              \
                __FILE__,                                                      \
                __LINE__,                                                      \
                __func__,                                                      \
                #condition);                                                   \
    } while (0) //;

#define ASSERT(condition)                                                      \
    do {                                                                       \
        if (!(condition))                                                      \
            autil_fatalf(                                                      \
                "[%s:%d][%s] ASSERT FAILURE: %s",                              \
                __FILE__,                                                      \
                __LINE__,                                                      \
                __func__,                                                      \
                #condition);                                                   \
    } while (0) //;

#endif // TEST_H_INCLUDED
