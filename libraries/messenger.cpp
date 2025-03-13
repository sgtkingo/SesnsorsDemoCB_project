/**
 * @file messenger.cpp
 * @brief Definition of the messenger interface and related global functions.
 * 
 * This header defines the global functions for message operations. It includes configuration
 * and exception handling support..
 * 
 * @copyright 2024 MTA
 * @author 
 * Ing. Jiri Konecny
 */

 #ifndef MESSANGER_HPP
 #define MESSANGER_HPP

#include "messenger.hpp"

#ifdef ARDUINO_H
    #include <Arduino.h>  ///< Include Arduino 
    #include <HardwareSerial.h> ///< Include Arduino Serial functions

    HardwareSerial UART1(UART1_PORT);

    void sendMessage(const std::string &message) {
        UART1.println(message.c_str());
    }
    
    std::string receiveMessage() {
        String msg = ""; // static so it persists between calls
        unsigned long startTime = millis();

        while (UART1.available() == 0 && (millis() - startTime) < UART_TIMEOUT) {
          // Wait until data arrives or timeout occurs
        }

        if (Serial.available() > 0) {
          // Read response
          msg = UART1.readString();
        } else {
          // Handle timeout situation
          // Do nothing
        }

        return std::string(msg.c_str());
    }
    
    void initMessenger(unsigned long baudrate = UART1_BAUDRATE, unsigned int mode = SERIAL_8N1, int tx = UART1_TX, int rx = UART1_RX) {
        UART1.begin(baudrate, mode, tx, rx);
        while(!UART1);
    }

    void initMessenger() {
        initMessenger(UART1_BAUDRATE, SERIAL_8N1, UART1_TX, UART1_RX);
    }

#elif defined(STDIO_H)
    #include <stdio.h>    ///< Include standard I/O functions

    void sendMessage(const std::string &message) {
        printf("%s\n", message.c_str());
    }

    std::string receiveMessage() {
        char buffer[256];
        scanf("%s", buffer);
        return std::string(buffer);
    }

    void initMessenger() {
        // No initialization needed for standard I/O
        return;
    }
    
#endif

#endif // MESSANGER_HPP