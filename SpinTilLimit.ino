//motor 1: short distance (x in 3d)
const int dirPin1 = 12;
const int stepPin1 = 13;
//motor 2: long distance (y in 3d)
const int dirPin2 = 10;
const int stepPin2 = 11;

const int in1 = 9;
const int in2 = 7;
const int in3 = 8;
const int in4 = 6;

const int limitSwitch1A = 1; // Motor 3 - Close end
const int limitSwitch1B = 2; // Motor 4 - Far end


const int limitSwitch2B = 3; // Motor 5 - Close end
const int limitSwitch2A = 4; // Motor 6 - Far end

bool direction1 = HIGH;
bool direction2 = HIGH;

long xPos = 0;
long yPos = 0;

int xMax = 2406;
int yMax = 1141;

void setup() {
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(limitSwitch1A, INPUT);
  pinMode(limitSwitch1B, INPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(limitSwitch2A, INPUT);
  pinMode(limitSwitch2B, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
  home();
}

void loop() {
  home();
  for(int i = 0; i< 100; i++){
  annoyingStepperForward();
  }
  delay(100);
  for (int j = 0; j< 100; j++){
  annoyingStepperBackward();
  }
  delay(100);
  
}

void step(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(500);
}
void slowStep(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(3000);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(3000);
}
void fastStep(int stepPin) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(100);
}


void home() {
  direction1 = HIGH;
  direction2 = LOW;

  //home y axis
  digitalWrite(dirPin1, direction1); // Set y direction toward home (bottom left)
  while (digitalRead(limitSwitch1A) == HIGH){ //while x axis is not home
    step(stepPin1);
    delayMicroseconds(500);
    }
  delay(100);
  direction1 = LOW; //backoff
  digitalWrite(dirPin1, direction1);
  for (int i = 0; i<100; i++){
    step(stepPin1);
    delayMicroseconds(500);
  }
  direction1 = HIGH;
  digitalWrite(dirPin1, direction1); // Set x direction toward home

  while (digitalRead(limitSwitch1A) == HIGH){ //while x axis is not home
    slowStep(stepPin1);
    delayMicroseconds(500);
    }

    xPos = 0;
    
  //home x axis
  digitalWrite(dirPin2, direction2); // Set y direction toward home
  while (digitalRead(limitSwitch2B) == HIGH){
    step(stepPin2);
    delayMicroseconds(500);
    }
  delay(100);
  direction2 = HIGH; //backoff
  digitalWrite(dirPin2, direction2);
  for (int i = 0; i<100; i++){
    step(stepPin2);
    delayMicroseconds(500);
  }
  direction2 = LOW;
  digitalWrite(dirPin2, direction2); // Set x direction toward home
  while (digitalRead(limitSwitch2B) == HIGH){
    slowStep(stepPin2);
    delayMicroseconds(500);
    }
    yPos = 0;
}

void axisLength(){
  direction1 = LOW;
  direction2 = HIGH;
  int xcount = 0;
  int ycount = 0;
  //get y axis length
  digitalWrite(dirPin1, direction1); // Set y directions away from home
  while (digitalRead(limitSwitch1B) == HIGH){ //while y axis is still travelling
    step(stepPin1);
    delayMicroseconds(500);
    ycount++;
    } 
    yMax = xcount;

    
  //get x axis length
  digitalWrite(dirPin2, direction2);
  while (digitalRead(limitSwitch2A) == HIGH){
    step(stepPin2);
    delayMicroseconds(500);
    xcount++;
    } 
    xMax = ycount;

}

void goTo(int targetX, int targetY) {
  int dx = abs(targetX - xPos);
  int dy = abs(targetY - yPos);
  int sx = (targetX > xPos) ? 1 : -1;
  int sy = (targetY > yPos) ? 1 : -1;

  // Update direction pins based on your motor setup
  digitalWrite(dirPin1, (sy > 0) ? LOW : HIGH);  // Y axis: up = LOW
  digitalWrite(dirPin2, (sx > 0) ? HIGH : LOW);  // X axis: right = HIGH

  int err = dx - dy;

  int x = xPos;
  int y = yPos;

  while (x != targetX || y != targetY) {
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x += sx;
      step(stepPin2);  // X axis step
    }
    if (e2 < dx) {
      err += dx;
      y += sy;
      step(stepPin1);  // Y axis step
    }
    delayMicroseconds(500); // adjust speed here
  }

  xPos = targetX;
  yPos = targetY;
}

void drawCenteredSquare(int squareSize, int x, int y) {
  int halfSize = squareSize / 2;
  int x1 = x - halfSize;
  int y1 = y - halfSize;
  int x2 = x + halfSize;
  int y2 = y + halfSize;

  goTo(x1, y1); // Bottom-left
  goTo(x2, y1); // Bottom-right
  goTo(x2, y2); // Top-right
  goTo(x1, y2); // Top-left
  goTo(x1, y1); // Back to start
}

#include <math.h>

void drawCenteredTriangle(int side, int x, int y) {
  int half = side / 2;
  float height = side * sqrt(3) / 2.0;

  int centerX = x;
  int centerY = y;

  int topX = centerX;
  int topY = centerY + height / 2;

  int bottomLeftX = centerX - half;
  int bottomLeftY = centerY - height / 2;

  int bottomRightX = centerX + half;
  int bottomRightY = centerY - height / 2;

  // Move through the triangle points
  goTo(bottomLeftX, bottomLeftY);
  goTo(bottomRightX, bottomRightY);
  goTo(topX, topY);
  goTo(bottomLeftX, bottomLeftY); // Close the triangle
}

void drawCircle(int centerX, int centerY, int radius, int resolution) {
  for (int i = 0; i <= resolution; i++) {
    float theta = 2 * PI * i / resolution;
    int x = centerX + radius * cos(theta);
    int y = centerY + radius * sin(theta);
    goTo(x, y);
  }
}

void pingPong(){
// === Motor 1 ===
  digitalWrite(dirPin1, direction1);

  if (direction1 == HIGH && digitalRead(limitSwitch1A) == LOW) {
    Serial.println("Motor 1 hit switch A, reversing");
    direction1 = LOW;
  } else if (direction1 == LOW && digitalRead(limitSwitch1B) == LOW) {
    Serial.println("Motor 1 hit switch B, reversing");
    direction1 = HIGH;
  }

  fastStep(stepPin1);

  // === Motor 2 ===
  digitalWrite(dirPin2, direction2);

  if (direction2 == HIGH && digitalRead(limitSwitch2A) == LOW) {
    Serial.println("Motor 2 hit switch A, reversing");
    direction2 = LOW;
  } else if (direction2 == LOW && digitalRead(limitSwitch2B) == LOW) {
    Serial.println("Motor 2 hit switch B, reversing");
    direction2 = HIGH;
  }

  fastStep(stepPin2);

  // Short delay so we don’t overload the processor
  delayMicroseconds(1000);
}

void annoyingStepperForward() {
  annoyingStep1();
  annoyingStep2();
  annoyingStep3();
  annoyingStep4();
}

void annoyingStepperBackward() {
  annoyingStep4();
  annoyingStep3();
  annoyingStep2();
  annoyingStep1();
}

void annoyingStep1() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2);
}
void annoyingStep2() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(2);
}
void annoyingStep3() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2);
}
void annoyingStep4() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2);
}
