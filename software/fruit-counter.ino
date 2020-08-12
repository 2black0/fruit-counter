#include <HX711.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <ezButton.h>

const int convPin = A2;
const int btnPin = A3;
const int trigPin = 2;
const int echoPin = 3;
const int s0Pin = 4;
const int s1Pin = 5;
const int s2Pin = 6;
const int s3Pin = 7;
const int soutPin = 8;
const int servo1Pin = 9;
const int servo2Pin = 10;
const int sckPin = 11;
const int doutPin = 12;
const int ledPin = 13;

// --------------------------------------------
// GANTI NILAI SESUAI DENGAN NILAI KALIBRASI
int redtopSP = 0;   // ganti dengan nilai merah maksimal kematangan buah
int redbotSP = 0;   // ganti dengan nilai merah minimal kematangan buah
int greentopSP = 0; // ganti dengan nilai hujau maksimal kematangan buah
int greenbotSP = 0; // ganti dengan nilai hijau minimal kematangan buah
int bluetopSP = 0;  // ganti dengan nilai biru maksimal kematangan buah
int bluebotSP = 0;  // ganti dengan nilai biru minimal kematangan buah
float calibration_factor = 344.10; // ganti dengan nilai kalibrasi
// --------------------------------------------

int weightValue = 0;
int distanceValue = 0;
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int totalcounter = 0;
int ripecounter = 0;
int smallcounter = 0;
int bigcounter = 0;

bool startStatus = false;
bool resetStatus = false;
bool onStatus = false;
bool colorStatus = false;
bool weightStatus = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 scale(doutPin, sckPin);
ezButton button(btnPin);
Servo servo1;
Servo servo2;

void init_device() {
  Serial.begin(9600);
  lcd.init();
  button.setDebounceTime(50);
  scale.set_scale();
  scale.tare();
  servo1.attach(servo1Pin, 600, 2300);
  servo2.attach(servo2Pin, 600, 2300);
  delay(50);
}

void init_pin() {
  pinMode(convPin, OUTPUT);
  pinMode(btnPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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
  lcd_show(1, 0, "Fish Counter", 1);
  lcd_show(0, 1, "by 2black0", 1000);
}

void loop() {
  button.loop();
  if (button.isReleased()) {
    startStatus = !startStatus;
  }

  if (startStatus) {
    converyor_on();
    on_process();
    if (onStatus) {
      converyor_off();
      color_process();
      if (colorStatus) {
        servo1_on();
      } else {
        servo1_off();
      }
      converyor_on();
      weight_process();
      if (weightStatus) {
        servo2_on();
      } else {
        servo2_off();
      }
    }
  } else {
    stop_process();
  }

  delay(1000);
}

void reset_var() {
  if (!resetStatus) {
    lcd_show(1, 0, "Clear All Var", 500);
    totalcounter = 0;
    ripecounter = 0;
    smallcounter = 0;
    bigcounter = 0;

    startStatus = false;
    onStatus = false;
    resetStatus = true;
    weightStatus = false;
  }
}

void on_process() {
  reset_var();
  distanceValue = read_ultrasonic();
  if (distanceValue < 30) {
    onStatus = true;
    totalcounter++;
    lcd_show(1, 0, "Counter:" + String(totalcounter), 1000);
  } else {
    onStatus = false;
  }
}

void color_process() {
  digitalWrite(s1Pin, LOW);
  digitalWrite(s2Pin, LOW);
  redValue = pulseIn(soutPin, LOW);
  delay(100);

  digitalWrite(s2Pin, HIGH);
  digitalWrite(s3Pin, HIGH);
  greenValue = pulseIn(soutPin, LOW);
  delay(100);

  digitalWrite(s2Pin, LOW);
  digitalWrite(s3Pin, HIGH);
  blueValue = pulseIn(soutPin, LOW);
  delay(100);

  if ((redValue >= redbotSP && redValue <= redbotSP) &&
      (redValue >= redbotSP && redValue <= redbotSP) &&
      (redValue >= redbotSP && redValue <= redbotSP)) {
    colorStatus = true;
    ripecounter++;
    lcd_show(1, 0, "Ripe OK", 1);
    lcd_show(0, 1, "Ripe" + String(ripecounter), 500);
  } else {
    colorStatus = false;
    lcd_show(1, 0, "Ripe Not OK", 1);
    lcd_show(0, 1, "Not Ripe" + String(totalcounter - ripecounter), 500);
  }
}

void weight_process() {
  weightValue = read_weight();
  if (weightValue > 10) {
    converyor_off();
    weightValue = read_weight();
    if (weightValue >= 100) {
      weightStatus = true;
      bigcounter++;
      lcd_show(1, 0, "B Fruit:" + String(bigcounter), 1000);
    } else {
      weightStatus = false;
      smallcounter++;
      lcd_show(1, 0, "S Fruit:" + String(smallcounter), 1000);
    }
  }
}

int read_weight() {
  scale.set_scale(calibration_factor);
  return (scale.get_units(), 4);
}

int read_ultrasonic() {
  long duration = 0;
  int distance = 0;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
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
  digitalWrite(conv, LOW);
  delay(100);
}

void converyor_off() {
  digitalWrite(conv, HIGH);
  delay(100);
}

void lcd_show(int clear, int lines, String text, int timedelay) {
  if (clear) {
    lcd.clear();
  }
  lcd.setCursor(0, lines);
  lcd.print(text);
  Serial.println(text);
  delay(timedelay);
}

void stop_process() {
  resetStatus = false;
  lcd_show(1, 0, "Press Button", 1);
  lcd_show(0, 1, "to Start", 2000);

  lcd_show(1, 0, "S Fish:" + String(smallcounter), 1);
  lcd_show(0, 1, "B Fish:" + String(bigcounter), 1000);
}