/*
  Drive
  Mini-Project #3
  
  The goal of project 3 was to make our own line-following wheeled robot.
  We created a two sensor scanner with IR sensors in order to distinguish
  the floor and a black piece of electrical tape. The wheeled robot makes
  slight adjustments left and right according to the readings that either
  of the two scanners produces.
  
  Part 1: Set up LEDs with a button-press switch to 
  change between several different modes.
  
  Part 2: Use potentiometer to change the brightness 
  of the LED output. 
  
  Coded by Toluope Oshinowo & Walter Villa with help from Leopoldo Sanchez & Aaron Huang
  October 21, 2021
  
  Principles of Integrated Engineering (PIE)
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(1);
// You can also make another motor on port M3
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);

const int analogInPin1 = A1; // Analog input - Left Motor
const int analogInPin2 = A2; // Analog input - Right Motor

int sensorValueLeftMotor = 0;         // value read from the pot
int sensorValueRightMotor = 0;         // value read from the pot

int rpmLeft = 25;    // variable to monitor the left motor's position
int rpmRight = 25;    // variable to  monitor the right motor's position

String modeInput;
String modeState;
int motorMode = 1;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  while (!Serial);
  Serial.println("Stepper test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  rightMotor->setSpeed(25);  // 25 rpm
  leftMotor->setSpeed(25);  // 25 rpm
}

void loop() {
  sensorValueLeftMotor = analogRead(analogInPin1);
  sensorValueRightMotor = analogRead(analogInPin2);

  if(Serial.available()){
    modeInput = Serial.readString();
    Serial.println(modeInput);
    if (modeInput != modeState) {
      modeState = modeInput;
      motorMode = !motorMode;
    }
  }
  
  if(motorMode == 1) { // normal mode
    if (sensorValueRightMotor < 600) { // if the tape is not detected move normally (backward = forward and vice versa)
      leftMotor->setSpeed(20);
      rpmLeft = 20;
      leftMotor->run(BACKWARD);
    } else if (sensorValueRightMotor > 600 && sensorValueRightMotor < 750 ) { // if the tape is somewhat detected stop moving so other moving wheel can perform a turn
      leftMotor->run(RELEASE);
      rpmLeft = 0;
    } else { // if the tape is heavily detected move in reverse and increase the rpm on the other wheel to perform a sharp turn with traction
      leftMotor->run(FORWARD);
      rpmLeft = -20;
      rightMotor->setSpeed(90);
      rightMotor->run(BACKWARD);
      rpmRight = 90;
    }
    if (sensorValueLeftMotor < 600) { // if the tape is not detected move normally (backward = forward and vice versa)
      rightMotor->setSpeed(20);
      rightMotor->run(BACKWARD);
      rpmRight = 20;
    } else if (sensorValueLeftMotor > 600 && sensorValueLeftMotor < 750 ) { // if the tape is somewhat detected stop moving so other moving wheel can perform a turn
      rightMotor->run(RELEASE);
      rpmRight = 0;
    } else { // if the tape is heavily detected move in reverse and increase the rpm on the other wheel to perform a sharp turn with traction
      rightMotor->run(FORWARD);
      rpmRight = -20;
      leftMotor->setSpeed(90);
      leftMotor->run(BACKWARD);
      rpmLeft = 90;
    }
  } else { // hellcat mode
    if (sensorValueRightMotor < 600) { // if the tape is not detected move normally (backward = forward and vice versa)
      leftMotor->setSpeed(60);
      rpmLeft = 60;
      leftMotor->run(BACKWARD);
    } else if (sensorValueRightMotor > 600 && sensorValueRightMotor < 750 ) { // if the tape is somewhat detected stop moving so other moving wheel can perform a turn
      leftMotor->run(RELEASE);
      rpmLeft = 0;
    } else { // if the tape is heavily detected move in reverse and increase the rpm on the other wheel to perform a sharp turn with traction
      leftMotor->run(FORWARD);
      rpmLeft = -60;
      rightMotor->setSpeed(180);
      rightMotor->run(BACKWARD);
      rpmRight = 180;
    }
    if (sensorValueLeftMotor < 600) { // if the tape is not detected move normally (backward = forward and vice versa)
      rightMotor->setSpeed(60);
      rightMotor->run(BACKWARD);
      rpmRight = 60;
    } else if (sensorValueLeftMotor > 600 && sensorValueLeftMotor < 750 ) { // if the tape is somewhat detected stop moving so other moving wheel can perform a turn
      rightMotor->run(RELEASE);
      rpmRight = 0;
    } else { // if the tape is heavily detected move in reverse and increase the rpm on the other wheel to perform a sharp turn with traction
      rightMotor->run(FORWARD);
      rpmRight = -60;
      leftMotor->setSpeed(180);
      leftMotor->run(BACKWARD);
      rpmLeft = 180;
    }
  }

  Serial.print(sensorValueLeftMotor); Serial.print(",");
  Serial.print(rpmLeft); Serial.print(",");
  Serial.print(sensorValueRightMotor); Serial.print(",");
  Serial.print(rpmRight); Serial.print(",");
  Serial.println("");
}
