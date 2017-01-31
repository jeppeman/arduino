#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT_OFF_THRESHOLD 200
#define MAX_ANALOG          1023
#define PHOTO_RES_PIN       A0
#define BUTTON_PIN          2
#define LED_PIN             6
#define NUM_PIXELS          24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB);
short buttonState = HIGH, prevButtonState = HIGH;
uint8_t brightness = 0;
uint32_t prevColor;

uint32_t randomizeColor() {
    return pixels.Color(
            (uint8_t) (rand() % 255),
            (uint8_t) (rand() % 255),
            (uint8_t) (rand() % 255)
    );
}

void colorWipe(bool changeColor) {
    bool hasChanged = false;
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, changeColor && !hasChanged ? prevColor = randomizeColor() : prevColor);
        hasChanged = true;
    }

    pixels.setBrightness(brightness);
    pixels.show();
}

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(PHOTO_RES_PIN, INPUT);
    pixels.begin();
    colorWipe(true);
}

void loop() {
    int photoResValue = analogRead(PHOTO_RES_PIN);
    if (photoResValue > LIGHT_OFF_THRESHOLD && photoResValue < MAX_ANALOG) {
        float offsetVal = photoResValue - LIGHT_OFF_THRESHOLD,
                offsetMax = MAX_ANALOG - LIGHT_OFF_THRESHOLD;
        brightness = (uint8_t) round((offsetVal / offsetMax) * 255);
        colorWipe(false);
    } else if (photoResValue < LIGHT_OFF_THRESHOLD) {
        brightness = 0;
        colorWipe(false);
    }

    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH && prevButtonState == LOW) {
        delay(20);
        buttonState = digitalRead(BUTTON_PIN);
        if (buttonState == HIGH && prevButtonState == LOW) {
            colorWipe(true);
        }
    }

    prevButtonState = buttonState;
    delay(100);
    Serial.println(brightness, DEC);
}