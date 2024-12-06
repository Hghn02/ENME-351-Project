#include <SharpIR.h> // Importing distance sensor library

// Importing IMU libraries
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <math.h> 

// Pin Numbers
int yawServoPin = 5;
int pitchServoPin =6;
int laserPin = 3;
 
// Servo PWM Parameters
const int t_min = 600; // Probably the minimum pulse width (datasheet had listed 900)
const int t_max = 2300; // Probably the maximum pulse width (datasheet had listed 2100)
const int servoPeriod = 20000; // Probably the servo period

// Mapped PWs from from orientation data
int mappedYawPW = 0;
int mappedPitchPW = 0;

// Laser Pulse Parameters
bool laserPulseState =false;

// Global variables for laser timing
unsigned long lastLaserToggle = 0;
bool laserState = false;
const unsigned long PULSE_PERIOD = 1000; 

// IMU Sensor Initialization
Adafruit_BNO055 bno = Adafruit_BNO055(55,0x28);
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Distance Sensor Initialization
SharpIR sensor( SharpIR::GP2Y0A41SK0F, A0 );

// Function to get distance for IR distance sensor
bool readDistance() {
  // Reading distance to a object
  int distanceToObj = sensor.getDistance();
  Serial.println(distanceToObj);

  // Target in range or not
  if (distanceToObj < 20) {
    return true;
  } else {
    return false;
  }
  
}

// Laser Control Function
void laserPulse(bool state) {
  unsigned long currentMicros = micros();
  
  if (state == true) { 
    // pulse implementation
    if (currentMicros - lastLaserToggle >= (PULSE_PERIOD/500)) { // Toggle every 500 microseconds
      laserState = !laserState;
      digitalWrite(laserPin, laserState);
      lastLaserToggle = currentMicros;
    }
  } else {
    digitalWrite(laserPin,HIGH); // No pulsing just pointing
  }
}

// Control Yaw and Pitch Servos simultaneously
void servoControl() {

  digitalWrite(yawServoPin,HIGH);
  digitalWrite(pitchServoPin, HIGH);
  delayMicroseconds(min(mappedYawPW, mappedPitchPW)); // Wait for the shorter pw

  if (mappedYawPW > mappedPitchPW) {
      digitalWrite(pitchServoPin, LOW); // Turn off the pitch servo
      delayMicroseconds(mappedYawPW - mappedPitchPW); // Wait for the remaining time for yaw
      digitalWrite(yawServoPin, LOW);
  } else {
      digitalWrite(yawServoPin, LOW); // Turn off the yaw servo
      delayMicroseconds(mappedPitchPW - mappedYawPW); // Wait for the remaining time for pitch
      digitalWrite(pitchServoPin, LOW);
  }

    // Ensure the total period is kept
  delayMicroseconds(servoPeriod - max(mappedYawPW, mappedPitchPW));

}

void readIMU() {

  // Fetching orientation and angular velocity data
  // Note the code below (lines 94 - 96) was borrowed from "https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code"
  sensors_event_t orientationData, angVelData;
  bno.getEvent(&orientationData,Adafruit_BNO055::VECTOR_EULER);
  //bno.getEvent(&angVelData, Adafruit_BNO055::VECTOR_GYROSCOPE);

  // Rounding yaw and pitch angles
  int yawAngle= round(orientationData.orientation.x);
  int pitchAngle = round(orientationData.orientation.z);
  int rollAngle = round(orientationData.orientation.y);


  // Mapping Euler angles to servo pulse range for actuation
  mappedYawPW = map(yawAngle,0,360,t_min,t_max);
  mappedPitchPW = map(constrain(pitchAngle,-90,0),-90,0,t_min,(t_max/2)+100);
  
  // Calculating actual Servo angles for Telemetry and verification
  int actualYawPW = map(mappedYawPW,t_min,t_max,0,360);
  int actualPitchPW = map(mappedPitchPW,t_min,(t_max/2)+100,-90,0);
  
  // Sending all orientation data over serial port to Processing
  Serial.print(yawAngle);
  Serial.print("\t");
  Serial.print(pitchAngle);
  Serial.print("\t");
  Serial.print(rollAngle);
  Serial.print("\t");
  Serial.print(actualYawPW);
  Serial.print("\t");
  Serial.print(actualPitchPW);
  Serial.print("\t");

}

void setup() {

  // Initializing pins
  Serial.begin(115200);

  pinMode(yawServoPin,OUTPUT);
  pinMode(pitchServoPin,OUTPUT);
  pinMode(laserPin,OUTPUT);

  digitalWrite(laserPin,LOW); 

  
  // Note the code below (lines 139 - 147) was borrowed from "https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/arduino-code"
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
  bno.setExtCrystalUse(true);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = micros();

  // Function calls 
  readIMU();
  servoControl();
  bool pulseState = readDistance();  
  laserPulse(pulseState);

  // Timing loop for entire system
  while ((micros() - currentTime) < (BNO055_SAMPLERATE_DELAY_MS * 1000))
  {

  }
}
