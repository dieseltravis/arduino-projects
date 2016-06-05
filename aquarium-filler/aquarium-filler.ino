#include <LowPower.h>

int relayPin = 2;
int ledPin = 3;

int sourceSensorPin = 7;
int sourceTriggerPin = 8;
int sourceSensorState = 0;

// ~16 seconds
int sourceDelay = 2;

int destSensorPin = 11;
int destTriggerPin = 12;
int destSensorState = 0;

// ~12 hours
int triggerDelay = 5400;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting aquarium filler...");
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(sourceSensorPin, INPUT);
  pinMode(sourceTriggerPin, OUTPUT);
  digitalWrite(sourceTriggerPin, LOW);

  pinMode(destSensorPin, INPUT);
  pinMode(destTriggerPin, OUTPUT);
  digitalWrite(destTriggerPin, LOW);
}

void loop() {
  digitalWrite(ledPin, LOW);

  // test source tank first
  digitalWrite(sourceTriggerPin, HIGH);
  sourceSensorState = digitalRead(sourceSensorPin);
  
  Serial.print("sourceSensorState: ");
  Serial.println(sourceSensorState);
  
  if (sourceSensorState == LOW){
    // Source tank is empty
    digitalWrite(relayPin, HIGH);
    digitalWrite(sourceTriggerPin, LOW);
    digitalWrite(ledPin, HIGH);
    
    Serial.println("Sleeping for ~16 seconds...");
    sleep_for(sourceDelay);
  }
  else if (sourceSensorState == HIGH){
    // source has water, test destination
    digitalWrite(destTriggerPin, HIGH);
    destSensorState = digitalRead(destSensorPin);
    
    Serial.print("destSensorState: ");
    Serial.println(destSensorState);
    
    if (destSensorState == LOW){
      // turn on pump relay
      digitalWrite(relayPin, LOW);
    }
    else if (destSensorState == HIGH){
      // Destination tank is full
      digitalWrite(relayPin, HIGH);
      digitalWrite(sourceTriggerPin, LOW);
      digitalWrite(destTriggerPin, LOW);
      
      Serial.println("Sleeping for ~12 hours...");
      sleep_for(triggerDelay);
    }
  }
}

void sleep_for(int eightSecondBlocks) {
  // wait for any serial messages to print before sleeping
  delay(1000);
  for (int i = 0; i < eightSecondBlocks; i++){
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
