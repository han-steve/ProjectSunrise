#include <AH_EasyDriver.h>

#define STEPPER_SPEED             35                  //Defines the speed in RPM for your stepper motor
#define STEPPER_STEPS_PER_REV     1028                //Defines the number of pulses that is required for the stepper to rotate 360 degrees
#define STEPPER_MICROSTEPPING     0                   //Defines microstepping 0 = no microstepping, 1 = 1/2 stepping, 2 = 1/4 stepping 
#define DRIVER_INVERTED_SLEEP     1                   //Defines sleep while pin high.  If your motor will not rotate freely when on boot, comment this line out.

#define STEPPER_DIR_PIN_BIG           12 //23 for the other motor
#define STEPPER_STEP_PIN_BIG          13 //22
#define STEPPER_SLEEP_PIN_BIG         14 //21

#define STEPPER_DIR_PIN_SMALL         23
#define STEPPER_STEP_PIN_SMALL        22
#define STEPPER_SLEEP_PIN_SMALL       21

// these are not being used.
#define STEPPER_MICROSTEP_1_PIN   18
#define STEPPER_MICROSTEP_2_PIN   19

AH_EasyDriver shadeStepperBig(STEPPER_STEPS_PER_REV, STEPPER_DIR_PIN_BIG ,STEPPER_STEP_PIN_BIG,STEPPER_MICROSTEP_1_PIN,STEPPER_MICROSTEP_2_PIN,STEPPER_SLEEP_PIN_BIG);
//AH_EasyDriver shadeStepperSmall(STEPPER_STEPS_PER_REV, STEPPER_DIR_PIN_SMALL ,STEPPER_STEP_PIN_SMALL,STEPPER_MICROSTEP_1_PIN,STEPPER_MICROSTEP_2_PIN,STEPPER_SLEEP_PIN_SMALL);

struct DEV_BigBlind : Service::WindowCovering {

  // homekit states
  SpanCharacteristic *current;
  SpanCharacteristic *target;

  bool moving = false;

  DEV_BigBlind() : Service::WindowCovering() {
    
    current = new Characteristic::CurrentPosition(0); // initialize as closed
    target = new Characteristic::TargetPosition(0);
    target->setRange(0, 100, 10);
    current->setRange(0, 100, 10);


    shadeStepperBig.setMicrostepping(STEPPER_MICROSTEPPING);            // 0 -> Full Step                                
    shadeStepperBig.setSpeedRPM(STEPPER_SPEED);     // set speed in RPM, rotations per minute
    #if DRIVER_INVERTED_SLEEP == 1
    shadeStepperBig.sleepOFF();
    #endif
    #if DRIVER_INVERTED_SLEEP == 0
    shadeStepperBig.sleepON();
    #endif
    
    Serial.print("Configuring Motorized Window Shade\n");
  }
  
  boolean update() {
    return(true);
  }

  // FORWARD = open, BACKWARD = close
  void step(bool forward, int amount) {
    LOG1("stepping, direction =" + forward);
    if (!moving) {
      #if DRIVER_INVERTED_SLEEP == 1
      shadeStepperBig.sleepON();
      #endif
      #if DRIVER_INVERTED_SLEEP == 0
      shadeStepperBig.sleepOFF();
      #endif
    }
    moving = true;
    shadeStepperBig.move(amount, !forward); // I'm reversing the direction to close the other way
  }
  
  void loop() {
    if (current->getVal() - target->getVal() >= 10) {
      Serial.print("closing blinds. current = ");
      Serial.print(current->getVal());
      Serial.print(" target = ");
      Serial.print(target->getVal());
      
      step(BACKWARD, 1200);
      if (current->getVal() == 10) {
        step(BACKWARD, 2000); // make sure we fully close the blinds
      }
      current->setVal(current->getVal() - 10); // we have stepped a whole revolution
                                                // which is 10 units on the 100 scale
    } else if (current->getVal() - target->getVal() <= -10) {
      Serial.print("opening blinds. current = ");
      Serial.print(current->getVal());
      Serial.print(" target = ");
      Serial.print(target->getVal());
      
      step(FORWARD, 1200);
      current->setVal(current->getVal() + 10);
    } else {
      if (moving) {
        #if DRIVER_INVERTED_SLEEP == 1
        shadeStepperBig.sleepOFF();
        #endif
        #if DRIVER_INVERTED_SLEEP == 0
        shadeStepperBig.sleepON();
        #endif

        moving = false;
      }
    }
  }
};
//
//struct DEV_SmallBlind : Service::WindowCovering {
//
//  // homekit states
//  SpanCharacteristic *current;
//  SpanCharacteristic *target;
//
//  bool moving = false;
//
//  DEV_SmallBlind() : Service::WindowCovering() {
//    
//    current = new Characteristic::CurrentPosition(0); // initialize as closed
//    target = new Characteristic::TargetPosition(0);
//    target->setRange(0, 100, 10);
//    current->setRange(0, 100, 10);
//
//
//    shadeStepperSmall.setMicrostepping(STEPPER_MICROSTEPPING);            // 0 -> Full Step                                
//    shadeStepperSmall.setSpeedRPM(STEPPER_SPEED);     // set speed in RPM, rotations per minute
//    #if DRIVER_INVERTED_SLEEP == 1
//    shadeStepperSmall.sleepOFF();
//    #endif
//    #if DRIVER_INVERTED_SLEEP == 0
//    shadeStepperSmall.sleepON();
//    #endif
//    
//    Serial.print("Configuring Motorized Window Shade\n");
//  }
//  
//  boolean update() {
//    return(true);
//  }
//
//  // FORWARD = open, BACKWARD = close
//  void step(bool forward, int amount) {
//    LOG1("stepping, direction =" + forward);
//    if (!moving) {
//      #if DRIVER_INVERTED_SLEEP == 1
//      shadeStepperSmall.sleepON();
//      #endif
//      #if DRIVER_INVERTED_SLEEP == 0
//      shadeStepperSmall.sleepOFF();
//      #endif
//    }
//    moving = true;
//    shadeStepperSmall.move(amount, forward);
//  }
//  
//  void loop() {
//    if (current->getVal() - target->getVal() >= 10) {
//      Serial.print("closing blinds. current = ");
//      Serial.print(current->getVal());
//      Serial.print(" target = ");
//      Serial.print(target->getVal());
//      
//      step(BACKWARD, 1200);
//      if (current->getVal() == 10) {
//        step(BACKWARD, 300); // make sure we fully close the blinds
//      }
//      current->setVal(current->getVal() - 10); // we have stepped a whole revolution
//                                                // which is 10 units on the 100 scale
//    } else if (current->getVal() - target->getVal() <= -10) {
//      Serial.print("opening blinds. current = ");
//      Serial.print(current->getVal());
//      Serial.print(" target = ");
//      Serial.print(target->getVal());
//      
//      step(FORWARD, 1200);
//      current->setVal(current->getVal() + 10);
//    } else {
//      if (moving) {
//        #if DRIVER_INVERTED_SLEEP == 1
//        shadeStepperSmall.sleepOFF();
//        #endif
//        #if DRIVER_INVERTED_SLEEP == 0
//        shadeStepperSmall.sleepON();
//        #endif
//
//        moving = false;
//      }
//    }
//  }
//};
