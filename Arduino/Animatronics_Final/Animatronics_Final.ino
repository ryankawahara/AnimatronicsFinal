/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>
#include <LiquidCrystal_I2C.h>


const int ledPin = 13;  // the pin that the LED is attached to
int incomingByte;       // a variable to read incoming serial data into
int switchVar = 0;
int countdownData[2];
size_t dataLength = sizeof(countdownData);
LiquidCrystal_I2C lcd(0x27,16,2);
bool isNorth = true;
bool northActionDone = false;
bool southActionDone = false;
bool armExtended = false;
bool hasBeenSwitched = false;
int pos = 0;    // variable to store the servo position

int northCountdown = -1;
int southCountdown = -1;



Servo arm_servo;  // create servo object to control a servo
Servo spin_servo;
// twelve servo objects can be created on most boards

int right_pos = 180;  // variable to store the servo position
int prev_rightpos;
int left_pos = 180;

bool firstRun = true;


void setup() {

  arm_servo.write(135);
  arm_servo.attach(2);  // attaches the servo on pin 9 to the servo object
  // retract_arm(arm_servo);
  Serial.begin(9600);


  // arm_servo.write(right_pos);
  pinMode(ledPin, OUTPUT);
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  lcd.clear();
  lcd.setCursor(0,0); // set the cursor to column 15, line 0
  lcd.print("York St");


  spin_servo.write(0);
  spin_servo.attach(12);
  // arm_servo.write(90);


}


void loop() {


    switchVar = digitalRead(5);

    // arm_servo.write(130);
    // arm_servo.write(60);


    int val1 = -1;
    int val2 = -1;

    lcd.clear();
    lcd.print("York St.");

    lcd.setCursor(0,1); // set the cursor to column 15, line 0

    if (switchVar == HIGH) {
      
      lcd.print("North");
      

      if (!northActionDone) { // Check if the action has not been performed
          setDial(spin_servo, 0);
          spin_servo.write(0);

          // spin_servo.write(0); // Then write 0 to the servo
          delay(1000);                       // waits 15 ms for the servo to reach the position

        digitalWrite(ledPin, HIGH);

      // if (!armExtended) {
      // extend_arm(arm_servo);
      // armExtended = true;
      // }

      if (armExtended) {
        retract_arm(arm_servo);
        armExtended = false;
        hasBeenSwitched = false;
      }

      if (northCountdown >= 0) {
          delay(500);                       // waits 15 ms for the servo to reach the position
          if (!armExtended) {
            extend_arm(arm_servo);
            armExtended = true;
          }
        delay(600); 
        setDial(spin_servo, mapValue(northCountdown));
      }

        northActionDone = true; // Set the action as done
      }

      southActionDone = false;


    }
    else {
      hasBeenSwitched = true;
      lcd.print("South");
      // setDial(spin_servo, mapValue(0));
                             // waits 15 ms for the servo to reach the position


      if (!southActionDone) {
        setDial(spin_servo, mapValue(0));


      //  if (!armExtended) {
      // extend_arm(arm_servo);
      // armExtended = true;
      // }
        delay(1000);                       // waits 15 ms for the servo to reach the position

        if (armExtended) {
            retract_arm(arm_servo);
            armExtended = false;
            hasBeenSwitched = false;
          }
          delay(600);                       // waits 15 ms for the servo to reach the position


      if (southCountdown >= 0) {
          delay(500);                       // waits 15 ms for the servo to reach the position
          if (!armExtended) {
            extend_arm(arm_servo);
            armExtended = true;
          }
        delay(600); 
        setDial(spin_servo, mapValue(southCountdown));
      }


      southActionDone = true;
      }
      // extend_arm(arm_servo);



      northActionDone = false; // Set the action as done

      digitalWrite(ledPin, LOW);

    }

  if (Serial.available() > 0) {

      if (!armExtended) {
      extend_arm(arm_servo);
      armExtended = true;
      }

    delay(1000);



    // when data is received

    String data = Serial.readStringUntil('\n');
    int commaIndex = data.indexOf(',');
    String firstNumber = data.substring(0, commaIndex);
    String secondNumber = data.substring(commaIndex + 1);

    // Convert the substrings to integers
    int num1 = firstNumber.toInt();
    int num2 = secondNumber.toInt();

    northCountdown = num1;
    southCountdown = num2;

    if (firstRun == false) {
      delay(5000);

    }
      if (switchVar == HIGH) {
        digitalWrite(ledPin, HIGH);

        setDial(spin_servo, mapValue(num1));
        // spin_servo.write(mapValue(num1));
      }
      else {
        lcd.print("South");

        digitalWrite(ledPin, LOW);
        // spin_servo.write(mapValue(num2));
        // spin_servo.write(mapValue(0));
        // setDial(spin_servo, mapValue(4));
      setDial(spin_servo, mapValue(num2));

      }

    
    firstRun = false;
    

  }





    
  delay(15); // optional delay to control servo update frequency

}



int mapValue(int input) {
  if (input < 0) input = 0;
  if (input > 10) input = 10;

  return map(input, 0, 10, 0, 145);
}

void extend_arm(Servo servo){
      int startPos = 60;
      int endPos = 130;

      for (pos = endPos; pos >= startPos; pos -= 1) { 
        servo.write(pos);              
        delay(20);                     
      }
}

void retract_arm(Servo servo) {
  int startPos = 60;
  int endPos = 130;
  for (pos = startPos; pos <= endPos; pos += 1) { // goes from 0 degrees to 180 degrees
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(20);                       // waits 15 ms for the servo to reach the position
  }
}

void setDial(Servo servo, int value) {
  int curr_pos = servo.read();
  
  // Increase the position if it's less than the target value
  while (curr_pos < value) {
    servo.write(curr_pos);
    curr_pos += 1;
    delay(20);     
  }

  // Decrease the position if it's greater than the target value
  while (curr_pos > value) {
    servo.write(curr_pos);
    curr_pos -= 1;
    delay(20);
  }
}
