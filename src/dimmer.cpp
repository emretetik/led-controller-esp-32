#include <Arduino.h>
#include "dimmer.h"

#define DIMMER_PIN 2
#define PWM_FREQUENCY 22000 // change later
#define PWM_DUTY_MAX 255
#define PWM_DUTY_MIN 0
#define STEP_SIZE 1
#define STEP_INTERVAL_MILLIS 12

#define MODE_PIN 4

int Dimmer::dimmerValue = 0;

void Dimmer::init() {
    ledcAttachPin(DIMMER_PIN, PWM_FREQUENCY);
    Dimmer::full();
    pinMode(MODE_PIN, OUTPUT);
}

void Dimmer::full() {
    Dimmer::setDimmerValue(PWM_DUTY_MAX);
}

void Dimmer::off() {
    Dimmer::setDimmerValue(PWM_DUTY_MIN);
}

void Dimmer::up(int target) {
    unsigned long previousMillis = 0UL;
    int currentVal = Dimmer::getDimmerValue();
    Serial.println("current val:");
    Serial.println(currentVal);
    for (;;) {
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis > STEP_INTERVAL_MILLIS) {
            if (currentVal + STEP_SIZE > target) {
                Dimmer::setDimmerValue(target);
                Serial.println("New val:");
                Serial.println(target);
                break;
            }
            else {
                currentVal += STEP_SIZE;
                Dimmer::setDimmerValue(currentVal);
                Serial.println("New val:");
                Serial.println(currentVal);
            }
            previousMillis = currentMillis;
        }
    }
}

void Dimmer::down(int target) {
    unsigned long previousMillis = 0UL;
    int currentVal = Dimmer::getDimmerValue();
    Serial.println("current val:");
    Serial.println(currentVal);
    for (;;) {
        unsigned long currentMillis = millis();
        if(currentMillis - previousMillis > STEP_INTERVAL_MILLIS) {
            if (currentVal - STEP_SIZE < target) {
                Dimmer::setDimmerValue(target);
                Serial.println("New val:");
                Serial.println(target);
                break;
            }
            else {
                currentVal -= STEP_SIZE;
                Dimmer::setDimmerValue(currentVal);
                Serial.println("New val:");
                Serial.println(currentVal);
            }
            previousMillis = currentMillis;
        }
    }
}

void Dimmer::setBrightness(int value) {
    
    // normalize input
    if (value < PWM_DUTY_MIN) {
        value = PWM_DUTY_MIN;
    }
    else if (value > PWM_DUTY_MAX) {
        value = PWM_DUTY_MAX;
    }

    int currentVal = Dimmer::getDimmerValue();

    if (value > currentVal) {
        Dimmer::up(value);
    }
    else if (value < currentVal) {
        Dimmer::down(value);
    }

}

void Dimmer::switchMode() {
    digitalWrite(MODE_PIN, !digitalRead(MODE_PIN));
}

void Dimmer::setDimmerValue(int value) {
    analogWrite(DIMMER_PIN, value);
    Dimmer::dimmerValue = value;
}

int Dimmer::getDimmerValue() {
    return Dimmer::dimmerValue;
}