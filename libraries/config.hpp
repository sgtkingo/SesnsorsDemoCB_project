/**
 * @file config.h
 * @brief Configuration file for platform-specific settings.
 * 
 * This file defines macros to select the execution environment (Arduino or standard console).
 * Uncomment the desired macro to enable the respective environment.
 * 
 * @copyright 2024 MTA
 * @author Ing. Jiri Konecny
 */

#ifndef CONFIG_H
#define CONFIG_H

/// Uncomment to enable logging for Arduino-based environments
// #define ARDUINO_H 
#define UART1_PORT 0
#define UART1_BAUDRATE 9600
#define UART1_RX -1
#define UART1_TX -1
#define UART_TIMEOUT 100


/// Uncomment to enable logging for standard console applications (PC/Linux)
#define STDIO_H 

#endif // CONFIG_H 