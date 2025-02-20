/**
 * @file logs.c
 * @brief Implementation of logging functions.
 */

#include "logs.hpp"
//#include <stdarg.h>  // For variadic arguments
#include <cstdarg>  // For variadic arguments
#include <cstdio>
#include <string>

#ifdef ARDUINO_H
    #include <Arduino.h>  ///< Include Arduino Serial functions
#elif defined(STDIO_H)
    #include <stdio.h>    ///< Include standard I/O functions
#endif

void logMessage(const char *format, ...) {
    va_list args;
    va_start(args, format);

    #ifdef ARDUINO_H
        // Create a buffer for formatted output
        char buffer[256];
        vsnprintf(buffer, sizeof(buffer), format, args);
        Serial.println(buffer);  // Print via Arduino Serial
    #elif defined(STDIO_H)
        vprintf(format, args);  // Print via standard console
    #endif

    va_end(args);
}

