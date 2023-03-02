/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "IRSensor.h"
#include "EncoderCounter.h"
#include "Controller.h"


// Blinking rate in milliseconds
#define BLINKING_RATE     250ms
#define kN                45


int main()
{
    // LEDs
    DigitalOut led0(PD_4); 
    DigitalOut led1(PD_3); 
    DigitalOut led2(PD_6); 
    DigitalOut led3(PD_2); 
    DigitalOut led4(PD_7); 
    DigitalOut led5(PD_5);

    //IRSensors
    AnalogIn distance(PA_0); 
    DigitalOut enable(PG_1); 
    DigitalOut bit0(PF_0); 
    DigitalOut bit1(PF_1); 
    DigitalOut bit2(PF_2);
    enable = 1; // schaltet die IR Distanzsensoren ein

    IRSensor irSensor0(distance, bit0, bit1, bit2, 0); 
    IRSensor irSensor1(distance, bit0, bit1, bit2, 1); 
    IRSensor irSensor2(distance, bit0, bit1, bit2, 2); 
    IRSensor irSensor3(distance, bit0, bit1, bit2, 3); 
    IRSensor irSensor4(distance, bit0, bit1, bit2, 4); 
    IRSensor irSensor5(distance, bit0, bit1, bit2, 5);

    //setup Motor Dirvers
    DigitalOut enableMotorDriver(PG_0); 
    DigitalIn motorDriverFault(PD_1); 
    DigitalIn motorDriverWarning(PD_0); 
    PwmOut pwmLeft(PF_9);
    PwmOut pwmRight(PF_8);

    pwmLeft.period(0.00005); // Setzt die Periode auf 50 μs 
    pwmRight.period(0.00005);
    pwmLeft = 0.5; // Setzt die Duty-Cycle auf 50% 
    pwmRight = 0.5;
    enableMotorDriver = 1; // Schaltet den Leistungstreiber ein

    //Setup encoder
    EncoderCounter counterLeft(PD_12, PD_13); 
    EncoderCounter counterRight(PB_4, PC_7);

    //Setup controller
    Controller controller(pwmLeft, pwmRight, counterLeft, counterRight);

    int timer = 0;

    while (true) {
        
        //set LEDs
        led0 = irSensor0 < 0.2f;
        led1 = irSensor1 < 0.2f;
        led2 = irSensor2 < 0.2f;
        led3 = irSensor3 < 0.2f;
        led4 = irSensor4 < 0.2f;
        led5 = irSensor5 < 0.2f;

        //Controller
        float forwardSpeed = 200.0;
        controller.setDesiredSpeedLeft(forwardSpeed); // Drehzahl in [rpm] 
        controller.setDesiredSpeedRight(-forwardSpeed);

        printf("actual speed (left/right): %.3f / %.3f [rpm]\r\n", controller.getActualSpeedLeft(), controller.getActualSpeedRight());
    
        ThisThread::sleep_for(BLINKING_RATE);
    }
}
