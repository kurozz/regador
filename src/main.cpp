#include <Arduino.h>

const int POT_VCC = A2;
const int POT_GND = A0;
const int POT_PIN = A1;
const int PUMP_PIN = 4;
const int LEVEL_PIN = 2;

const bool PUMP_ON = LOW;
const bool PUMP_OFF = HIGH;
const long PUMP_ON_TIME_MIN = 1000;
const long PUMP_ON_TIME_MAX = 60000;

const int TIMES_PER_DAY = 4;
const long TIME_BEFORE_RESET = (24L*60L*60L*1000L)/TIMES_PER_DAY;

void(* softReset) (void) = 0;

void setup() {
    pinMode(POT_VCC, OUTPUT);
    digitalWrite(POT_VCC, HIGH);

    pinMode(POT_GND, OUTPUT);
    digitalWrite(POT_VCC, HIGH);

    pinMode(POT_PIN, INPUT);

    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(PUMP_PIN, PUMP_OFF);

    pinMode(LEVEL_PIN, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, digitalRead(LEVEL_PIN));

    Serial.begin(9600);  
}

void loop() {
    uint32_t pot_val = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        pot_val += analogRead(POT_PIN);
    }

    uint32_t on_time = map(pot_val, 0, 4092, PUMP_ON_TIME_MIN, PUMP_ON_TIME_MAX);
    Serial.print("On time: ");
    Serial.print(on_time);
    Serial.println("ms");

    if (digitalRead(LEVEL_PIN))
    {
        digitalWrite(PUMP_PIN, PUMP_ON);
        delay(on_time);
        digitalWrite(PUMP_PIN, PUMP_OFF);
    }

    while (millis() < TIME_BEFORE_RESET);

    softReset();
}
