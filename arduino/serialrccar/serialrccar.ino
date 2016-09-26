
// Define PINs on Arduino
const int rightPin = 2;
const int forwardPin = 10;
const int backPin = 11;
const int leftPin =3;
const int ledPin = 13;

// Setup variables
unsigned long previousTimeMs = 0;
int fwdPulseMs = 100;
int fwdPulseLowMs = 500;
int fwdPulseHighMs = 250;

// Define starting conditions
int left = HIGH;
int right = HIGH;
int forward = HIGH;
int back = HIGH;
int error = HIGH;
int forwardPulse = HIGH;

void setup() {
  // Start Serial interface
  Serial.begin(9600);
  // Declare PINs as output
  pinMode(forwardPin, OUTPUT);
  pinMode(backPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  unsigned long now = millis();

  // Handling for forward pulse
  if (now - previousTimeMs > fwdPulseMs) {
    if (forward == LOW) {
      if (forwardPulse == LOW) {
        forwardPulse = HIGH;
        fwdPulseMs = fwdPulseLowMs;
      } else {
        forwardPulse = LOW;
        fwdPulseMs = fwdPulseHighMs;
      }
      digitalWrite(forwardPin, forwardPulse);
    }
    previousTimeMs = now;
  }

  // Incoming control handler. 
  
  int incomingByte;
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    left = right = forward = back = HIGH;
    if (incomingByte & 0x01) {
      left = LOW;
    }
    if (incomingByte & 0x02) {
      right = LOW;
    }
    if (incomingByte & 0x04) {
      forward = LOW;
    }
    if (incomingByte & 0x08) {
      back = LOW;
    }

    // Error handling
    if ((left == LOW && right == LOW) ||
        (forward == LOW && back == LOW)) {
      left = HIGH;
      right = HIGH;
      forward = HIGH;
      back = HIGH;
      error = HIGH;
    }

    // Actual output to board based on above logic
    digitalWrite(leftPin, left);
    digitalWrite(rightPin, right);
    if (forward == HIGH) {
      digitalWrite(forwardPin, forward);
    }
    digitalWrite(backPin, back);
    digitalWrite(ledPin, error);
  }
}
