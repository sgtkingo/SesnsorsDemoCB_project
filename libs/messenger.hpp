/**
 * @file messenger.hpp
 * @brief Declaration of the messenger interface and related global functions.
 * 
 * This header defines the abstract interface IMessenger for sending and receiving
 * messages, as well as global functions for message operations. It includes configuration
 * and exception handling support.
 * 
 * @copyright 2024 MTA
 * @author 
 * Ing. Jiri Konecny
 */

 #ifndef MESSENGER_HPP
 #define MESSENGER_HPP
 
 #include "config.hpp"     ///< Configuration.
 #include "exceptions.hpp" ///< Exception handling.
 #include <string>
 
 /**
  * @class IMessenger
  * @brief Abstract interface for a messenger.
  *
  * This interface declares methods for initializing the messenger,
  * sending messages, and receiving messages. Derived classes should
  * implement these methods according to the specific communication protocol.
  */
 class IMessenger {
 public:
     /**
      * @brief Constructs a new IMessenger object.
      */
     IMessenger() {}
 
     /**
      * @brief Virtual destructor.
      */
     virtual ~IMessenger() {}
 
     /**
      * @brief Initializes the messenger.
      * 
      * This method performs necessary initialization steps.
      * @throws Exception if initialization fails.
      */
     virtual void initMessenger() = 0;
 
     /**
      * @brief Sends a message.
      * 
      * @param message The message to send.
      * @throws Exception if sending fails.
      */
     virtual void sendMessage(const std::string &message) = 0;
 
     /**
      * @brief Receives a message.
      * 
      * @return A string containing the received message.
      * @throws Exception if receiving fails.
      */
     virtual std::string receiveMessage() = 0;
 };
 
 /**
  * @brief Sends a message using the global messenger.
  * 
  * @param message The message to send.
  * @throws Exception if sending fails.
  */
 void sendMessage(const std::string &message);
 
 /**
  * @brief Receives a message using the global messenger.
  * 
  * @return A string containing the received message.
  * @throws Exception if receiving fails.
  */
 std::string receiveMessage();
 
/**
* @brief Initializes the global messenger.
* 
* @param baudrate The baudrate for the messenger.
* @param mode The mode for the messenger.
* @param tx The transmit pin for the messenger.
* @param rx The receive pin for the messenger.
* @throws Exception if initialization fails.
*/
 void initMessenger(unsigned long baudrate, unsigned int mode, int tx, int rx);

  /**
  * @brief Initializes the global messenger.
  * 
  * @throws Exception if initialization fails.
  */
 void initMessenger();
 
 #endif // MESSENGER_HPP
 