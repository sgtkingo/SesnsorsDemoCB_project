/**
 * @file messenger.cpp
 * @brief Definition of the messenger interface and related global functions.
 * 
 * This source defines the abstract interface IMessenger for sending and receiving
 * messages, as well as global functions for message operations. It includes configuration
 * and exception handling support.
 * 
 * @copyright 2024 MTA
 * @author 
 * Ing. Jiri Konecny
 */

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
        if (UART1.available() > 0) {
            msg = UART1.readString();
        }
        return std::string(msg.c_str());
    }
    
    void initMessenger(unsigned long baudrate = UART1_BAUDRATE, unsigned int mode = SERIAL_8N1, int tx = UART1_TX, int rx = UART1_RX) {
        UART1.begin(baudrate, mode, tx, rx);
        while(!UART1);
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
