#DiegoMartinez 5/31/17

#include <TimedAction.h>
#include <Servo.h>


Servo servo1, servo2, servo3, servo4, servo5, servo6;
char flex1 = A0, flex2 = A1, flex3 = A2, flex4 = A3, flex5 = A4, flex6 = A5;
short val1, val2, val3, val4, val5, val6;

const char buttonPinPlay = 2, buttonPinRecord = 3;
bool playButtonState = true, recordButtonState = true; //pullup resistor always true
const char ledPinGreen = 4, ledPinRed = 5;

bool playState = 0, recordState = 0;
short indexCounter = 0, usedArraySize = 0;
#define arrayCapacity 100
short servo1_arr [arrayCapacity];
short servo2_arr [arrayCapacity];
short servo3_arr [arrayCapacity];
short servo4_arr [arrayCapacity];
short servo5_arr [arrayCapacity];
short servo6_arr [arrayCapacity];

void record();
void manual();
TimedAction recordThread = TimedAction(60,record);
TimedAction manualThread = TimedAction(50,manual);

void setup() {
  // put your setup code here, to run once:
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
  servo5.attach(12);
  servo6.attach(13);
  Serial.begin(9600);
  
  pinMode(buttonPinPlay, INPUT_PULLUP);
  pinMode(buttonPinRecord, INPUT_PULLUP);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinRed , LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  if(playState) { //PLAY MODE
    Serial.println("PLAY MODE");
    indexCounter = 0;
    while(indexCounter <= usedArraySize) {
      indexCounter++;
      //move servos
      servo1.write(servo1_arr[indexCounter]);
      servo2.write(servo2_arr[indexCounter]);
      servo3.write(servo3_arr[indexCounter]);
      servo4.write(servo4_arr[indexCounter]);
      servo5.write(servo5_arr[indexCounter]);
      servo6.write(servo6_arr[indexCounter]);
      delay(60);
    }
    playState = false;
    digitalWrite(ledPinGreen, LOW);
    manualThread.enable();
    recordThread.enable();
    //reloops again to beginning of loop()
  }
  else {
    playButtonState = digitalRead(buttonPinPlay);
    if(playButtonState == 0) {
      Serial.println("PLAY SETUP");
      digitalWrite(ledPinGreen, HIGH);
      digitalWrite(ledPinRed, LOW);
      manualThread.disable();//Terminate Manual Thread
      recordThread.disable();//Terminate record thread
      playState = true;
      //reloop loop()
    }
    else { //MANUAL THREAD
      //Serial.println("MANUAL");
      manualThread.check();//manual();
      if(recordState && usedArraySize < arrayCapacity) {
        Serial.print("Record: " );
        Serial.println(usedArraySize);
        recordThread.check();//record();
        if(usedArraySize >= arrayCapacity){
          digitalWrite(ledPinRed, LOW);
          recordState = false;
        }
      }
      else {
        recordButtonState = digitalRead(buttonPinRecord);
        if(recordButtonState == 0) {
          Serial.println("RECORD BUTTON PRESS");
          recordState = true;
          usedArraySize = 0;
          digitalWrite(ledPinRed, HIGH);
          recordThread.check();//record();
        }
        //else do nothing
      }
    }
  }
}
void record() {
      servo1_arr[usedArraySize] = val1;
      servo2_arr[usedArraySize] = val2;
      servo3_arr[usedArraySize] = val3;
      servo4_arr[usedArraySize] = val4;
      servo5_arr[usedArraySize] = val5;
      servo6_arr[usedArraySize] = val6;
      usedArraySize++;
}
void manual() {
      //read potentiometer values
      val1 = analogRead(flex1);
      val2 = analogRead(flex2);
      val3 = analogRead(flex3);
      val4 = analogRead(flex4);
      val5 = analogRead(flex5);
      val6 = analogRead(flex6);
      //map to servo
      val1 = map(val1,790,850,0,180);
      val2 = map(val2,800,900,0,180);
      val3 = map(val3,860,900,0,180);
      val4 = map(val4,920,950,0,180);
      val5 = map(val5,810,900,0,180);
      val6 = map(val6,853,860,0,180);
      //write to servo
      servo1.write(val1);
      servo2.write(val2);
      servo3.write(val3);
      servo4.write(val4);
      servo5.write(val5);
      servo6.write(val6);
}
