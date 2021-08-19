#ifndef LOGGER_H
#define LOGGER_H

#include <errno.h>
#include <stdio.h>
#include<string.h>


#define LOG_RED "\033[31m"
#define LOG_YELLOW "\033[33m"
#define LOG_GREEN "\033[32m"
#define LOG_WHITE "\033[37m"
#ifdef NDEBUG
#define debug(MSG, ...)
#else
#define debug(MSG, ...)                                               \
    fprintf(stderr, "[DEBUG] (%s:%d): " MSG "\n", __FILE__, __LINE__, \
            ##__VA_ARGS__)
#endif

#define log_err(MSG, ...)                                             \
    fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " MSG "\n", __FILE__, \
            __LINE__, errno == 0 ? "None" : strerror(errno), ##__VA_ARGS__)
#define log_warn(MSG, ...)                                              \
    fprintf(stderr, "[WARNING] (%s:%d: errno: %s) " MSG "\n", __FILE__, \
            __LINE__, errno == 0 ? "None" : strerror(errno), ##__VA_ARGS__)

#endif
