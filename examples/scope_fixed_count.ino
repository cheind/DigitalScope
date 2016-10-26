/**
    This file is part of DigitalScope - a software for capturing digital input events.

    Copyright(C) 2016 Christoph Heindl

    All rights reserved.
    This software may be modified and distributed under the terms
    of the BSD license.See the LICENSE file for details.

    This example collects a fixed number of input events from a single input pin.
    Once the collection is complete it prints the results via Serial. 

    DigitalScope uses carefully designed interrupt service routines to maximize
    the frequency of observable event changes.
*/

#include "Arduino.h"

// Include the library
#include <DigitalScope.h>
using namespace cheind;

// Initalize scope with number of events to collect (128) and target pin (2)
#define NEVENTS 128
DigitalScope<NEVENTS> scope(2);

// Will be used to signal data readiness. 
bool complete = false;

void setup()
{
    Serial.begin(9600);
    while(!Serial) {}

    // Set a callback function when data recording is complete. 
    scope.setCompletedCallback(onComplete);

    // Start recording.
    complete = false;
    scope.start();
}

void loop()
{
    if (complete) {
        
        // Data is available, print via Serial
        
        Serial.println("BEGIN DATA");
        for (uint16_t i = 0; i < NEVENTS; ++i)
        {
            // Print info about event time in microseconds 
            // since first event, the current state HIGH/LOW, the event type triggering
            // the state change RISING/FALLING.
            Serial.print(scope.timeOf(i)); Serial.print(",");            
            Serial.print(scope.stateOf(i)); Serial.print(",");
            Serial.print(scope.eventOf(i));
            Serial.println();
        }
        Serial.println("END DATA");
        
        // Restart the scope after a short pause.

        delay(5000);
        Serial.println("LOG Ready for capture");
        complete = false;
        scope.start();
    }
}

/**
    Callback function invoked by scope when required number of samples were recorded.

    This function is handed to scope through DigitalScope::setCompletedCallback in 
    setup(), before the scope is started.     
*/
void onComplete() 
{
    // Signal loop() that data is available.
    complete = true;
}