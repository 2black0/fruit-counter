/*
tutorial kalibrasi:
https://randomnerdtutorials.com/arduino-color-sensor-tcs230-tcs3200/
*/

const int s0Pin = 4;
const int s1Pin = 5;
const int s2Pin = 6;
const int s3Pin = 7;
const int soutPin = 8;

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  Serial.begin(9600);
  pinMode(s0Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s2Pin, OUTPUT);
  pinMode(s3Pin, OUTPUT);
  pinMode(soutPin, INPUT);

  digitalWrite(s0Pin, HIGH);
  digitalWrite(s1Pin, LOW);
}
void loop() {
  digitalWrite(s2Pin, LOW);
  digitalWrite(s3Pin, LOW);
  redFrequency = pulseIn(soutPin, LOW);
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);

  digitalWrite(s2Pin, HIGH);
  digitalWrite(s3Pin, HIGH);
  greenFrequency = pulseIn(soutPin, LOW);
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);

  digitalWrite(s2Pin, LOW);
  digitalWrite(s3Pin, HIGH);
  blueFrequency = pulseIn(soutPin, LOW);
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  delay(100);
}
