/**
 * Author Teemu Mäntykallio
 * Initializes the library and turns the motor in alternating directions.
*/
//purple is 32+2

#define EN_PIN    9 // 38  // Nano v3:	16 Mega:	38	//enable (CFG6)
#define DIR_PIN   10 // 34  //19			55	//direction
#define STEP_PIN  11 //32  //18			54	//step
#define CS_PIN    12 //36  //17			64	//chip select

bool dir = true;


#include <TMC2130Stepper.h>
TMC2130Stepper TMC2130 = TMC2130Stepper(EN_PIN, DIR_PIN, STEP_PIN, CS_PIN);

void setup() {
  Serial.begin(9600);
  TMC2130.begin(); // Initiate pins and registeries
  TMC2130.SilentStepStick2130(600); // Set stepper current to 600mA
  TMC2130.stealthChop(1); // Enable extremely quiet stepping
  
  digitalWrite(EN_PIN, LOW);
}

void loop() {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(150);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(150);
  // uint32_t ms = millis();
  // static uint32_t last_time = 0;
  // if ((ms - last_time) > 2000) {
  //   if (dir) {
  //     Serial.println("Dir -> 0");
  //     TMC2130.shaft_dir(0);
  //   } else {
  //     Serial.println("Dir -> 1");
  //     TMC2130.shaft_dir(1);
  //   }
  //   dir = !dir;
  //   Serial.println(TMC2130.GCONF(), BIN);
  //   last_time = ms;
  // }
}
