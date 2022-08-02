#ifndef _LOG_H_
#define _LOG_H_
#define LOG(x)            \
    {                     \
        printf(__FILE__); \
        printf(__LINE__); \
        printf(x);        \
        printf('\n');     \
    }                     \
#endif