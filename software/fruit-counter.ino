#include <HX711_ADC.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int sckPin = A0;
const int doutPin = A1;
const int convPin = A2;
const int S1trigPin = 2;
const int S1echoPin = 3;
const int s0Pin = 4;
const int s1Pin = 5;
const int s2Pin = 6;
const int s3Pin = 7;
const int soutPin = 8;
const int servo1Pin = 9;
const int servo2Pin = 10;
const int S2trigPin = 11;
const int S2echoPin = 12;
const int ledPin = 13;

// --------------------------------------------
// GANTI NILAI SESUAI DENGAN NILAI KALIBRASI
int redtopSP = 0;   // ganti dengan nilai merah maksimal kematangan buah
int redbotSP = 0;   // ganti dengan nilai merah minimal kematangan buah
int greentopSP = 0; // ganti dengan nilai hujau maksimal kematangan buah
int greenbotSP = 0; // ganti dengan nilai hijau minimal kematangan buah
int bluetopSP = 0;  // ganti dengan nilai biru maksimal kematangan buah
int bluebotSP = 0;  // ganti dengan nilai biru minimal kematangan buah
float calibrationValue = 1.0; // ganti dengan new calibration value
// --------------------------------------------

int weightValue = 0;
int distance1Value = 0;
int distance2Value = 0;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int totalCounter = 0;
int ripeCounter = 0;
int smallCounter = 0;
int bigCounter = 0;

bool weightStatus = false;

static boolean newDataReady = 0;
const int serialPrintInterval = 0;
long t;

LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711_ADC LoadCell(doutPin, sckPin);
Servo servo1;
Servo servo2;

void init_hx711() {
  LoadCell.begin();
  long stabilizingtime = 2000;
  boolean _tare = false;
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Check LoadCell");
    lcd_show(1, 0, "Check LoadCell", 500);
    while (1)
      ;
  } else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("LoadCell OK");
    lcd_show(1, 0, "LoadCell OK", 500);
  }
}

void init_device() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  servo1.attach(servo1Pin, 600, 2300);
  servo2.attach(servo2Pin, 600, 2300);
  init_hx711();
  delay(50);
}

void init_pin() {
  pinMode(convPin, OUTPUT);
  pinMode(S1trigPin, OUTPUT);
  pinMode(S1echoPin, INPUT);
  pinMode(S2trigPin, OUTPUT);
  pinMode(S2echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(s0Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s2Pin, OUTPUT);
  pinMode(s3Pin, OUTPUT);
  pinMode(soutPin, INPUT);

  digitalWrite(convPin, HIGH);
  digitalWrite(s0Pin, HIGH);
  digitalWrite(s1Pin, LOW);
  delay(50);
}

void init_servo() {
  servo1_off();
  servo2_off();
  delay(50);
}

void setup() {
  init_device();
  init_pin();
  init_servo();
  Serial.println("Fruit Counter by 2black0");
  lcd_show(1, 0, "Fruit Counter", 1);
  lcd_show(0, 1, "by 2black0", 1000);
}

void loop() {
  distance1Value = read_ultrasonic(S1trigPin, S1echoPin);
  Serial.println("Distance1:" + String(distance1Value));
  if (distance1Value < 25) {
    converyor_off();
    distance1Value = 0;
    totalCounter++;
    Serial.println("Total:" + String(totalCounter));

    redValue = red_color();
    greenValue = green_color();
    blueValue = blue_color();
    if ((redValue >= redbotSP && redValue <= redtopSP) &&
        (greenValue >= greenbotSP && redValue <= greentopSP) &&
        (blueValue >= bluebotSP && redValue <= bluetopSP)) {
      ripeCounter++;
      Serial.println("Ripe Counter:" + String(ripeCounter));
    } else {
      Serial.println("unRipe Counter:" + String(totalCounter - ripeCounter));
      servo1_on();
    }
  }

  converyor_on();
  distance2Value = read_ultrasonic(S2trigPin, S2echoPin);
  Serial.println("Distance2:" + String(distance2Value));
  if (distance2Value < 25) {
    distance2Value = 0;
    weightStatus = true;
  }
  if (LoadCell.update())
    newDataReady = true;

  if (newDataReady && weightStatus) {
    if (millis() > t + serialPrintInterval) {
      weightValue = LoadCell.getData();
      newDataReady = 0;
      weightStatus = 0;
      t = millis();

      if (weightValue < 8538676) {
        smallCounter++;
        led_on();
        servo2_on();
        Serial.println("Weight <= 200 gram");
        Serial.println("S Fruit:" + String(smallCounter));
        led_off();
      } else if (weightValue >= 8538676) {
        bigCounter++;
        led_on();
        servo2_off();
        Serial.println("Weight > 200 gram");
        Serial.println("B Fruit:" + String(bigCounter));
        led_off();
      }
    }
  }

  lcd_show(1, 0, "T:" + String(totalCounter) + "R:" + String(ripeCounter), 1);
  lcd_show(0, 1, "S:" + String(smallCounter) + " B:" + String(bigCounter), 100);
}

int red_color() {
  digitalWrite(s1Pin, LOW);
  digitalWrite(s2Pin, LOW);
  int red = pulseIn(soutPin, LOW);
  delay(150);
  return red;
}

int green_color() {
  digitalWrite(s2Pin, HIGH);
  digitalWrite(s3Pin, HIGH);
  int green = pulseIn(soutPin, LOW);
  delay(150);
  return green;
}

int blue_color() {
  digitalWrite(s2Pin, LOW);
  digitalWrite(s3Pin, HIGH);
  int blue = pulseIn(soutPin, LOW);
  delay(150);
  return blue;
}

int read_ultrasonic(int TrigPin, int EchoPin) {
  Serial.println("Read Distance");
  long duration;
  int distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void servo1_on() {
  servo1.write(90);
  delay(1000);
}

void servo1_off() {
  servo1.write(45);
  delay(1000);
}

void servo2_on() {
  servo2.write(90);
  delay(1000);
}

void servo2_off() {
  servo2.write(45);
  delay(1000);
}

void converyor_on() {
  digitalWrite(convPin, LOW);
  delay(100);
}

void converyor_off() {
  digitalWrite(convPin, HIGH);
  delay(100);
}

void led_on() {
  Serial.println("Led ON");
  digitalWrite(ledPin, LOW);
}

void led_off() {
  Serial.println("Led OFF");
  digitalWrite(ledPin, HIGH);
}

void lcd_show(int clear, int lines, String text, int timedelay) {
  if (clear) {
    lcd.clear();
  }
  lcd.setCursor(0, lines);
  lcd.print(text);
  delay(timedelay);
}