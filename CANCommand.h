/**
 * CANCommand - A Wiring/Arduino library to tokenize and parse commands
 * received over CANBus.
 * 
 * Copyright (C) 2018 Adam Coddington
 * 
 * Derived directly from SerialCommand created by:
 * 
 * Copyright (C) 2012 Stefan Rado
 * Copyright (C) 2011 Steven Cogswell <steven.cogswell@gmail.com>
 *                    http://husks.wordpress.com
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <Arduino.h>

class CANCommand {
  public:
    CANCommand();      // Constructor

    void addCommand(uint32, void(*function)());  // Add a command to the processing dictionary.
    void setDefaultHandler(void (*function)());   // A handler to call when no valid command received.

    struct CANMessage {
      uint32 ID;
      uint8 IDE;
      uint8 RTR;
      uint8 DLC;
      uint8 Data[8];
      uint8 FMI;
    };
    CANMessage *message;

    void getData(uint8_t[]);
    void processCANMessage(CANMessage* incoming_message);
  private:
    // Command/handler dictionary
    struct CANCommandCallback {
      uint32 command;
      void (*function)();
    };                                    // Data structure to hold Command/Handler function key-value pairs
    CANCommandCallback *commandList;   // Actual definition for command/handler array
    byte commandCount;

    // Pointer to the default handler function
    void (*defaultHandler)();
};
