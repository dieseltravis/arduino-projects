int relayPin = 2;
int ledPin = 3;

int sourceSensorPin = 9;
int sourceTriggerPin = 10;
int sourceSensorState = 0;

// 10 seconds
int sourceDelay = 10000;

int destSensorPin = 11;
int destTriggerPin = 12;
int destSensorState = 0;

// 12 hours
int triggerDelay = 12 * 60 * 60 * 1000;

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
    // TODO: warn to fill source tank (blink LEDs?
    digitalWrite(relayPin, HIGH);
    digitalWrite(sourceTriggerPin, LOW);
    digitalWrite(ledPin, HIGH);
    Serial.println("Sleeping for 10 seconds...");
    delay(sourceDelay);
  }
  else if (sourceSensorState == HIGH){
    // source has water, test destination
    digitalWrite(destTriggerPin, HIGH);
    destSensorState = digitalRead(destSensorPin);
    Serial.print("destSensorState: ");
    Serial.println(destSensorState);
    if (destSensorState == LOW){
      // turn on pump
      digitalWrite(relayPin, LOW);
    }
    else if (destSensorState == HIGH){
      digitalWrite(relayPin, HIGH);
      digitalWrite(sourceTriggerPin, LOW);
      digitalWrite(destTriggerPin, LOW);
      Serial.println("Sleeping for 12 hours...");
      delay(triggerDelay);
    }
  }
}
