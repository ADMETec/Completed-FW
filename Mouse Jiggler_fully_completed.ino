#include <avr/sleep.h>
#include <avr/power.h>

const int buttonPin = PIN_PC2;
const int ledPin = PIN_PC0;
const int ledPin1 = PIN_PC1;
const int ledPin2 = PIN_PA3;
const int batteryPin = PIN_PA1;  // Analog pin for battery voltage measurement
const int motorPin1 = PIN_PB1;
const int motorPin2 = PIN_PB0;

int buttonState = HIGH;
int lastButtonState = HIGH;
int batteryHealth;  // Declare batteryHealth as a global variable
unsigned long buttonPressTime = 0;
bool systemOn = false;
unsigned long motorStartTime = 0;

const unsigned long motorDuration = 1000;
const unsigned long motorInterval = 1000;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(PIN_PC3, OUTPUT);
  pinMode(batteryPin,INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(PIN_PC3,LOW);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(PIN_PB2, HIGH);

  // Battery health detection
  analogReference(INTERNAL1V1);  // Set the reference voltage to 1.1V
  delay(1000);

  attachInterrupt(buttonPin, btnCheck, FALLING);
  delay(1000);
  LEDPattern();
}

void btnCheck() {
  buttonState = !buttonState;
}

void loop() {
  digitalWrite(ledPin, LOW);

  while (!buttonState) {
    
    // Call the BatteryHealth function
    BatteryHealth();

    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    delay(1000);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    delay(600);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(500);

    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    delay(400);
  }

  // Turn off the device and enter sleep mode
  turnOff();
  sleepNow();
}

void BatteryHealth(){

  int batteryHealth = analogRead (batteryPin);
  
    if (batteryHealth > 800) {
    // Battery health over 800, turn on green light
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin1, LOW);
  } else if (batteryHealth > 680 && batteryHealth <= 800) {
    // Battery health between 650 and 800, turn on yellow light
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  } else if (batteryHealth <=680) {
    // Battery health under 650, turn on red light
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin, LOW);
  }
}

void LEDPattern() {
  fadeRGB(255, 0, 0);
  fadeRGB(255, 255, 0);
  fadeRGB(0, 255, 0);
  fadeRGB(0, 255, 255);
  fadeRGB(0, 0, 255);
  fadeRGB(255, 0, 255);
  fadeRGB(255, 255, 255);
  turnOff();
}

void fadeRGB(int red, int green, int yellow) {
  int delayTime = 1000;
  int steps = 100;

  for (int i = 0; i <= steps; i++) {
    analogWrite(ledPin, (yellow * i) / steps);
    analogWrite(ledPin1, (red * i) / steps);
    analogWrite(ledPin2, (green * i) / steps);
    delay(delayTime / steps);
  }
}

void turnOff() {
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  delay(1000);
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
}
