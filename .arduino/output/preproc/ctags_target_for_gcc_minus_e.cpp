# 1 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
# 2 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino" 2
# 3 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino" 2

Zumo32U4Encoders encoders;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Motors motors;
Zumo32U4OLED oled;



                  ///////////////////
                  //// Encoders /////
                  ///////////////////

//---Encoders Variabel---//
int32_t countLeft = 0;
int32_t countRight = 0;

void setupEncoders() {
  encoders.init();
}

void updateEncoders() {
  long Left = encoders.getCountsAndResetLeft();
  long Right = encoders.getCountsAndResetRight();

  countLeft += Left;
  countRight += Right;

}

                ////////////////////////
                // Calculate Distance //
                ////////////////////////

//------Global Speed & Distance Variablers ------//
const float WHEEL_DIAMETER_METER = 0.035; // 35 mm converted to meters
const float wheelCircumference = 2 * 3.1415926535897932384626433832795 * WHEEL_DIAMETER_METER;
const float encoderCpr = 909.7 ; // Counts per revolution
int prevCountLeft = 0;
int prevCountRight = 0;
//-----------------------------------------------//


float distance = 0;

void updateDistance(){
  float distanceLeft = (countLeft * wheelCircumference) / encoderCpr;
  float distanceRight = (countRight * wheelCircumference) / encoderCpr;

  distance = (distanceRight + distanceLeft) / 2;
}


                    ///////////////////
                    // Calculate Speed
                    ///////////////////

//----Speed-Variabler----//
float speedLeft = 0;
float speedRight = 0;
const float timeInterval = 1.0;
//-----------------------//


float speed = 0;

void updateSpeed() {
  int deltaCountLeft = countLeft - prevCountLeft;
  int deltaCountRight = countRight - prevCountRight;

  speedLeft = (deltaCountLeft * wheelCircumference) / (encoderCpr * timeInterval);
  speedRight = (deltaCountRight * wheelCircumference) / (encoderCpr * timeInterval);

  prevCountRight = countRight;
  prevCountLeft = countLeft;

  speed = (speedLeft + speedRight) / 2.0;
}

                    ///////////////////
                    // Average Speed //
                    ///////////////////

//---Average-Speed-Variables---//
const int numMeasurements = 10;
float speedMeasurements[numMeasurements] = {0};
int speedIndex = 0;
//-----------------------------//

float averageSpeed = 0;

void updateAverageSpeed(float speed) {
  speedMeasurements[speedIndex] = speed;
  speedIndex = (speedIndex + 1) % numMeasurements;

  float sum = 0;
  for (int i = 0; i < numMeasurements; i++){
    sum += speedMeasurements[i];
  }
  averageSpeed = sum / numMeasurements;
}



                    ///////////////////
                    ////OLED-Screen////
                    ///////////////////

void setupOLED(){
  oled.init();
  oled.clear();
}

void updateSpeedOMeterScreen(){
  oled.setLayout11x4();
  oled.gotoXY(2.5, 0);
  oled.print("SpeedY");

  oled.gotoXY(1, 2);

  oled.print("m/s: ");
  oled.print(speed);

  oled.gotoXY(2, 3);

  oled.print("m: ");
  oled.print(distance);
}



                      ///////////////////
                      // Discharge Rate /
                      ///////////////////

//-----Discharge-Variables-----//
const float dischargeRateBase = 0.05;
//-----------------------------//


float dischargeRate = 0;

void updateDischargeRate() {
  updateDistance();
  updateAverageSpeed(speed);

  float calculatedDischargeRate = dischargeRateBase * averageSpeed * distance;

  dischargeRate += calculatedDischargeRate/1.5;
}


                      ///////////////////
                      // Charge Rate /
                      ///////////////////

//-----Discharge-Variables-----//
const float chargeRateBase = 0.05;
//-----------------------------//


float chargeRate = 0;

void updateChargeRate() {
  updateDistance();
  updateAverageSpeed(speed);

  float calculatedChargeRate = chargeRateBase * averageSpeed * distance;

  chargeRate += calculatedChargeRate/2;
}



                      ///////////////////
                      ////// Alarm //////
                      ///////////////////

int alarm = 0;

void updateAlarm() {

}


                      ///////////////////
                      // Battery Level //
                      ///////////////////


//-----Battery-Variables-----//
const float maxBatteryLevel = 100.0;
const float minBatteryLevel = 0.0;
const float lowBatteryThreshold = 20.0;
//-----------------------------//

//-----------Flag-MOVEMENT-------------//
bool shouldMove = true;
//-------------------------------------//


float batteryLevel = 100.0;

void updateBatteryLevel() {

  if (speed > 0){
    batteryLevel -= dischargeRate;

    if (batteryLevel < lowBatteryThreshold){
      batteryLevel = lowBatteryThreshold;
    }
  }

  if (buttonB.isPressed()){
    shouldMove = false;

    motors.setSpeeds(-150, -150);
    batteryLevel += chargeRate;

    if (batteryLevel > maxBatteryLevel){
      batteryLevel = maxBatteryLevel;
    }
  }

  else {
  shouldMove = true;
  }

}


                      /////////////////////
                      // Charging cycles //
                      /////////////////////


int chargingCycles = 0;

void updateChargingCycle(){
  chargingCycles++;
}

///////////////////
//// Main Loop ////
///////////////////

void setup() {
  Serial.begin(9600);
  buttonA.waitForPress();
  setupOLED();
  setupEncoders();
}

void loop() {
  if (shouldMove){
    motors.setSpeeds(150, 150);
  }

  updateEncoders(); //Fixed

/*

  //-----Encoder-OUTPUT-------//

  Serial.println("--------------");

  Serial.println("CountLeft: ");

  Serial.println(countLeft);

  Serial.println("CountRight: ");

  Serial.println(countRight);

  Serial.println("--------------");

  //--------------------------//

 */
# 274 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateDistance(); //Fixed 

/*

  //-------Distance-OUTPUT-------//

  Serial.println("Distance: ");

  Serial.println(distance);

  Serial.println("--------------");

  //-----------------------------//

*/
# 284 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateSpeed(); //Fixed

/*

  //-------Speed-OUTPUT-------//

  Serial.println("Speed: ");

  Serial.println(speed);

  Serial.println("--------------");

  //-----------------------------//

*/
# 295 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateAverageSpeed(speed); //Fixed 
/*

  //-------Averagespeed-OUTPUT-------//

  Serial.println("Averagespeed: ");

  Serial.println(averageSpeed);

  Serial.println("--------------");

  //-----------------------------//

*/
# 304 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateDischargeRate(); //Fixed

/*

  //-------Discharge-OUTPUT-------//

  Serial.println("DischargeRate: ");

  Serial.println(dischargeRate);

  Serial.println("--------------");

  //-----------------------------//

*/
# 314 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateChargeRate(); //Fixed

/*

  //-------Discharge-OUTPUT-------//

  Serial.println("ChargeRate: ");

  Serial.println(chargeRate);

  Serial.println("--------------");

  //-----------------------------//  

*/
# 323 "C:\\Users\\yamns\\Documents\\Studysession\\IELS\\IELS\\Zumo\\Ny mappe\\ok\\ok.ino"
  updateSpeedOMeterScreen();

  updateBatteryLevel();
  Serial.println("BatteryLevel: ");
  Serial.println(batteryLevel);
  Serial.println("---------------");

  updateChargingCycle();
  Serial.println("ChargingCylce: ");
  Serial.println(chargingCycles);


//---------------------------------------TEST-logic------------------------------------//

  delay(500);
}
