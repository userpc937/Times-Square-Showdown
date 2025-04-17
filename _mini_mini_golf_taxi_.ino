const int AIN1 = 13;
const int AIN2 = 12;
const int PWMA = 11;

int motorSpeed = 255;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  randomSeed(analogRead(A0));  // to randomize pause times
}

void loop() {
  // Spin forward for 3 seconds
  spinMotor(motorSpeed);
  delay(3000);

  // Stop motor
  spinMotor(0);

  // Random pause time between 0.5 and 3.5 seconds
  int pauseTime = random(500, 3500);
  delay(pauseTime);
}

void spinMotor(int speed) {
  if (speed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else if (speed < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }

  analogWrite(PWMA, abs(speed));
}
