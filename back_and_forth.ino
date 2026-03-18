const int dirPin1 = 12;
const int stepPin1 = 13;
const int dirPin2 = 10;
const int stepPin2 = 11;

void setup() {
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
}

void loop() {
  // Move forward
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, HIGH);

  for (long i = 0; i < 700; i++) {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);

    delayMicroseconds(1000);  // adjust speed
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);  // wait 1 second at end

  // Move backward
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);

  for (long i = 0; i < 700; i++) {
    digitalWrite(stepPin1, HIGH);
    digitalWrite(stepPin2, HIGH);

    delayMicroseconds(1000);  // adjust speed
    digitalWrite(stepPin1, LOW);
    digitalWrite(stepPin2, LOW);
    delayMicroseconds(1000);
  }

  delay(1000);  // wait 1 second
}
