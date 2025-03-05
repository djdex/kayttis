#include <ezButton.h>

const int buzPin = 7;
ezButton keySwitch(8);
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 11;
float timing = 0.0;
float distance = 0.0;
int keyState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzPin, OUTPUT);

  keySwitch.setDebounceTime(50);
  digitalWrite(trigPin, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  keySwitch.loop();

  digitalWrite(trigPin, LOW);
  delay(2);
  
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  timing = pulseIn(echoPin, HIGH);
  distance = (timing * 0.034) / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  
  keyState = keySwitch.getState();
  if (distance < 15 && keyState == LOW) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
    digitalWrite(buzPin, LOW);
  }

  Serial.print("cm | ");
  Serial.print(distance / 2.54);
  Serial.println("in");
  delay(100);
}
