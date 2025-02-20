/**
 * @file logs.h
 * @brief Header file for logging functions.
 */

 #ifndef LOGS_H
 #define LOGS_H
 
 #include "config.hpp"  ///< Configuration file inclusion
 
 /**
  * @brief Logs a formatted message to the appropriate output (Serial for Arduino, stdout for PC).
  * 
  * This function supports variadic arguments similar to `printf()`.
  * 
  * @param format The format string (like in printf).
  * @param ... Additional arguments for formatting.
  */
 void logMessage(const char *format, ...);

 
 #endif // LOGS_H
 