#include <ezButton.h>

const int buzzPin = 7;
ezButton keySwitch(8);
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 11;
ezButton sleepBut(12);
float timing = 0.0;
float distance = 0.0;
int keyState = 0;

const int doorDistance = 20;
bool doorOpened = false;
//Time
const int keyWait = 5000; //Oikeassa 5min
int prevTime = 0;
int proxTimer = 0;
int sleepTimer = 0;
const int sleepTime = 5000;
int deltaTime = 0;
int keysTakenTime = 0;

void decrementTimer() {
  proxTimer = max(proxTimer - deltaTime, 0);
  sleepTimer = max(sleepTimer - deltaTime, 0);

  if (keyState == LOW) {
    keysTakenTime = 0;
  } else {
    keysTakenTime += deltaTime;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  keySwitch.setDebounceTime(50);
  sleepBut.setDebounceTime(50);
  digitalWrite(trigPin, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  deltaTime = millis() - prevTime;
  prevTime = millis();
  decrementTimer();

  
  keySwitch.loop();
  sleepBut.loop();

  if (proxTimer == 0) {
    digitalWrite(trigPin, LOW);

    delay(2);
    
    digitalWrite(trigPin, HIGH);

    delay(10);

    digitalWrite(trigPin, LOW);
    timing = pulseIn(echoPin, HIGH);
    distance = (timing * 0.034) / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    Serial.print(keysTakenTime);
    Serial.println("ms since keys last taken");
    proxTimer = 100;
  }

  keyState = keySwitch.getState();

  Serial.println(sleepBut.getState());
  if (sleepBut.getState() == HIGH) {
    sleepTimer = sleepTime;
  }

  
  if (distance > doorDistance && sleepTimer == 0) {
    doorOpened = true;
    if (keyState == LOW) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzPin, HIGH);
    } else {
      if (keysTakenTime > keyWait) {
        //Tässä voisi laittaa hälyn pois päältä, kunnes ovi suljetaan taas seuraavan kerran
        digitalWrite(ledPin, HIGH);
        digitalWrite(buzzPin, LOW);
      } 
    }  
  } else {
    doorOpened = false;
    
    digitalWrite(buzzPin, LOW);

    if (keyState == LOW || sleepTimer != 0) {
      digitalWrite(ledPin, LOW);
    }
  }
}
