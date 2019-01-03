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
 * Version 20120522
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
#include "CANCommand.h"

/**
 * Constructor makes sure some things are set.
 */
CANCommand::CANCommand()
  : commandList(NULL),
    commandCount(0),
    defaultHandler(NULL)
{
}

/**
 * Adds a "command" and a handler function to the list of available commands.
 * This is used for matching a found token in the buffer, and gives the pointer
 * to the handler function to deal with it.
 */
void CANCommand::addCommand(uint32 command, void (*function)()) {
  commandList = (CANCommandCallback *) realloc(commandList, (commandCount + 1) * sizeof(CANCommandCallback));
  commandList[commandCount].command = command;
  commandList[commandCount].function = function;
  commandCount++;
}

void CANCommand::getData(uint8_t data[]) {
  if(message == NULL) {
    return;
  }

  for(uint8 i = 0; i < message->DLC; i++) {
    data[i] = message->Data[i];
  }
}

/**
 * This sets up a handler to be called in the event that the receveived command string
 * isn't in the list of commands.
 */
void CANCommand::setDefaultHandler(void (*function)(CANMessage* msg)) {
  defaultHandler = function;
}

void CANCommand::processCANMessage(CANMessage* incoming_message) {
  bool matched = false;
  message = incoming_message;

  for(uint8 i =0; i < commandCount; i++) {
    if(commandList[i].command == message->ID) {
      matched = true;

      (*commandList[i].function)();
    }
  }
  if(!matched && (defaultHandler != NULL)) {
    (*defaultHandler)(incoming_message);
  }
}
