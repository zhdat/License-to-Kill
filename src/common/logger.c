/*
 * Author: Babacar (https://stackoverflow.com/users/3599371)
 */

#include "logger.h"

/**
 * \file logger.c
 * \brief Contains functions for logging.
 */

void log_format(const char* tag, const char* message, va_list args) {
    time_t now;
    time(&now);
    char* date = ctime(&now);
    date[strlen(date) - 1] = '\0';
#if DEBUG
    printf("%s [%s] ", date, tag);
#endif
    vprintf(message, args);
#if DEBUG
    printf("\n");
#endif
}

void log_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
#if DEBUG
    printf(RED);
#endif
    log_format("Error", message, args);
#if DEBUG
    printf(RESET "");
#endif
    va_end(args);
}

void log_info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_format("Info", message, args);
    va_end(args);
}

void log_debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
#if DEBUG
    printf(CYN);
#endif
    log_format("Debug", message, args);
#if DEBUG
    printf(RESET "");
#endif
    va_end(args);
}
