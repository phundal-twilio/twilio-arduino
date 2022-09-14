/*********
  Some code borrowed from https://randomnerdtutorials.com/esp32-dual-core-arduino-ide/  
*********/

#include <Arduino.h>
#include <IRremote.hpp>

// Create a Task Handler for Task1. This will be used for the IR receiver on its own core
TaskHandle_t Task1;

// IR receiver setup for the NEC protocol on GPIO 15
#define DECODE_NEC
#define IR_RECEIVE_PIN  15

// We'll call this the "IR Receiving Flag". It will keep state of te IR command receipt
bool IRCommandReceived = false;

void setup() {
  Serial.begin(115200);

  // Set up IR module via 3v3, GND, and GPIO 15
  Serial.println("");
  IrReceiver.begin(IR_RECEIVE_PIN);
  Serial.println("");
  Serial.print("IR receiver at pin ");
  Serial.println(IR_RECEIVE_PIN);
  Serial.print("Waiting for IR command");
  Serial.println("");
  
  // Create a task that will be executed in the Task1code() function, with priority 0 on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task- saw avg. of 9120 at highest in testing */
                    NULL,        /* parameter of the task */
                    0,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500);

  /*
   * Set up modem's cellular connectivity, open data channel, prep for data sending
   */
  
}

//Task1code: Catches IR commands
void Task1code( void * pvParameters ){

//UBaseType_t uxHighWaterMark; // Stack size debugger to go with the below in the for loop

  for(;;){

    /* Uncomment get the stack size (for debugging if the stack size needs to be increased)
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Stack size debugger
    Serial.print("Stack size High Water Mark: ");
    Serial.println(uxHighWaterMark);
    Serial.println("");
    */
    
    // Check if received data is available and if it is, try to decode it.
    if (IrReceiver.decode()) {
      Serial.print("EXECUTED ON CORE: ");
      Serial.println(xPortGetCoreID());

      // Check that the IR command is a code from a known protocol. If not, don't set the IR Receiving flag
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise via IR. Try getting closer or resending."));
        Serial.println();
      } else {
        // Set IR receiving flag to true. This tells core 1 that a command was received
        Serial.println("IR Command Received");
        Serial.println("");
        
        IRCommandReceived = true;
      }

    // Debounce timer to reduce duplicate commands
    delay(500);

      // !!!Important!!! Enable receiving of the next value
      IrReceiver.resume(); // Enable receiving of the next value
    }
  }
}

// Arduino sketch loop() runs on core 1 by default, so we'll put the IR command parsing and connectivity here
void loop() {
  // Check IR Receiving flag
  if (IRCommandReceived){
    Serial.print("EXECUTED ON CORE: ");
    Serial.println(xPortGetCoreID());

    // Trigger actions for certain IR commands
    if (IrReceiver.decodedIRData.command == 0x4D) {
      Serial.println("Button 1 pressed");
    } else if (IrReceiver.decodedIRData.command == 0x16) {
      Serial.println("Button 2 pressed");
    } else {
      Serial.println("Some other button pressed");
    }

    // Print a short summary of IR command received
    Serial.print("  Details: ");
    IrReceiver.printIRResultShort(&Serial);
    Serial.println("");
    
    // Reset IR receiving flag
    IRCommandReceived = false;

    /*
     * Do cellular stuff here! 
     */
     
  }
}
