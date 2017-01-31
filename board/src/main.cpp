#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Pulse.h>

#define STRIP_ONE_PIN       6
#define STRIP_TWO_PIN       5
#define BUTTON_PIN          2
#define NUM_PIXELS          120

Adafruit_NeoPixel stripOnePixels = Adafruit_NeoPixel(NUM_PIXELS, STRIP_ONE_PIN, NEO_GRBW),
        stripTwoPixels = Adafruit_NeoPixel(NUM_PIXELS, STRIP_TWO_PIN, NEO_GRBW);
short buttonState = HIGH, prevButtonState = HIGH;
bool on = true, stop = false;
const uint8_t pulseLength = 8;
uint32_t prevColor;
uint8_t prevRed, prevGreen, prevBlue, laps = 5, currLap = 0, currIndex =
        (stripOnePixels.numPixels() + stripTwoPixels.numPixels()) - 1;
Pulse *pulses[120 * 2 / pulseLength];
ArduinoTimer *timer;
float prevF = (float) 0.0, prevF2 = (float) 1.0, prevF3 = (float) 0.0;

uint32_t randomizeColor() {
    uint32_t ret = stripOnePixels.Color(
            prevRed = (uint8_t) (rand() % 255),
            prevGreen = (uint8_t) (rand() % 255),
            prevBlue = (uint8_t) (rand() % 255));
//    Serial.println(prevRed);
//    Serial.println(prevGreen);
//    Serial.println(prevBlue);
    return ret;
//    return stripOnePixels.Color(prevRed = (uint8_t) 255, prevGreen = (uint8_t) 0, prevBlue = (uint8_t) 255, (int8_t) 0);
}

void colorWipe(bool changeColor, bool wipe) {
    bool hasChanged = false;
    for (uint16_t i = 0; i < stripOnePixels.numPixels(); i++) {
        if (!on) {
            stripOnePixels.setPixelColor(i, stripOnePixels.Color(0, 0, 0, 0));
            stripTwoPixels.setPixelColor(i, stripTwoPixels.Color(0, 0, 0, 0));
        } else {
            stripOnePixels.setPixelColor(i, changeColor && !hasChanged ? prevColor = randomizeColor() : prevColor);
            stripTwoPixels.setPixelColor(i, changeColor && !hasChanged ? prevColor = randomizeColor() : prevColor);
//            stripOnePixels.show();
//            stripTwoPixels.show();
//            delay(10);
        }

        if (wipe) {
            stripOnePixels.setPixelColor(i, stripOnePixels.Color(0, 0, 0, 0));
            stripTwoPixels.setPixelColor(i, stripTwoPixels.Color(0, 0, 0, 0));
        }

        hasChanged = true;
    }

    stripOnePixels.show();
    stripTwoPixels.show();
}

bool reset = false;

void bulletIsh() {
    if (stop) {
//        randomizeColor();
        for (Pulse *pulse : pulses) {
//            pulse->setRGB((uint8_t) (rand() % 255), (uint8_t) (rand() % 255), (uint8_t) (rand() % 255));
            pulse->reset();

        }
        pulses[0]->fire();
        timer->countdown_ms(200);
        prevF = 0.0;
        prevF2 = 1.0;
        prevF3 = 0.0;
        stop = false;
        currIndex = (stripOnePixels.numPixels() + stripTwoPixels.numPixels()) - 1;
        currLap = 0;
        reset = true;
    }

//    if (currLap == 1) {
//        if (prevF >= 0) {
//            for (uint16_t i = 0; i < stripOnePixels.numPixels(); i++) {
//                if (!on) continue;
//
//                stripOnePixels.setPixelColor(i, prevRed, prevGreen, prevBlue, (uint8_t) round(prevF * 255));
//                stripTwoPixels.setPixelColor(i, prevRed, prevGreen, prevBlue, (uint8_t) round(prevF * 255));
////        stripOnePixels.setPixelColor(i, stripOnePixels.Color(0, 0, 0, 0));
////        stripTwoPixels.setPixelColor(i, stripTwoPixels.Color(0, 0, 0, 0));
//            }
//            stripOnePixels.show();
//            stripTwoPixels.show();
//            prevF -= 0.1;
//            delay(3);
//        } else if (prevF2 >= 0) {
//            for (uint8_t k = 255; k >= 0; k--) {
//                for (uint16_t i = 0; i < stripOnePixels.numPixels(); i++) {
//                    if (!on) continue;
//
//
//                    stripOnePixels.setPixelColor(i, (uint8_t) prevRed, (uint8_t) k,
//                                                 prevBlue, 0);
//                    stripTwoPixels.setPixelColor(i, prevRed, (uint8_t) k,
//                                                 prevBlue, 0);
//
////        stripOnePixels.setPixelColor(i, stripOnePixels.Color(0, 0, 0, 0));
////        stripTwoPixels.setPixelColor(i, stripTwoPixels.Color(0, 0, 0, 0));
//                }
//                stripOnePixels.show();
//                stripTwoPixels.show();
//
//                if (k <= 0) {
//                    prevF2 = -1;
//                    break;
//                }
//
//
//            }
//
//
//        } else {
//            for (uint8_t k = 0; k <= 255; k++) {
//                for (uint16_t i = 0; i < stripOnePixels.numPixels(); i++) {
//                    if (!on) continue;
//
//
//                    stripOnePixels.setPixelColor(i, (uint8_t) 0, (uint8_t) 0,
//                                                 k, 0);
//                    stripTwoPixels.setPixelColor(i, (uint8_t) 0, (uint8_t) 0,
//                                                 k, 0);
////        stripOnePixels.setPixelColor(i, stripOnePixels.Color(0, 0, 0, 0));
////        stripTwoPixels.setPixelColor(i, stripTwoPixels.Color(0, 0, 0, 0));
//                }
//
//                stripOnePixels.show();
//                stripTwoPixels.show();
//                if (k >= 255) {
//                    stop = true;
//                    break;
//                }
//            }
//
//
//        }
//
//
//        return;
//    }
    int count = 0;
    for (Pulse *pulse : pulses) {
        count++;
        if (pulse->isStopped() && pulse->isFinalized()) continue;
        if (!pulse->hasFired()) continue;


//        if (pulse->getOffset() - pulse->getLength() >= 0) {
        if (pulse->isStopped()) {
//            for (uint16_t i = currIndex; i < currIndex + pulse->getLength(); i++) {
//                if (i >= stripOnePixels.numPixels()) {
//                    int x = i - stripOnePixels.numPixels();
//
//                    stripTwoPixels.setPixelColor(stripTwoPixels.numPixels() - x,
//                                                 stripOnePixels.Color(prevRed, prevGreen, prevBlue, 0));
////            Serial.println(stripTwoPixels.numPixels() - x, DEC);
//
//                } else {
//                    stripOnePixels.setPixelColor(i, stripOnePixels.Color(prevRed, prevGreen, prevBlue, 0));
////        stripOnePixels.show();
//                }
//            }
            pulse->finalize();
            continue;
        }

        if (pulse->getOffset() - pulse->getLength() >= 0) {
            for (uint16_t i = pulse->getOffset() - pulse->getLength(); i < pulse->getOffset(); i++) {
                if (!on || pulse->isFinalized()) continue;

                if (i == currIndex) {
                    for (uint8_t j = currIndex; j < currIndex + pulse->getLength(); j++) {
                        if (j >= stripOnePixels.numPixels()) {
                            int x = j - stripOnePixels.numPixels();

                            stripTwoPixels.setPixelColor(stripTwoPixels.numPixels() - x,
                                                         stripOnePixels.Color(pulse->getR(), pulse->getG(), pulse->getB(), 0));
//            Serial.println(stripTwoPixels.numPixels() - x, DEC);

                        } else {
                            stripOnePixels.setPixelColor(j, stripOnePixels.Color(pulse->getR(), pulse->getG(), pulse->getB(), 0));
//        stripOnePixels.show();
                        }
                    }
                    pulse->finalize();
                    break;
                }

                if (count >= 29) {
                    continue;
                }

                if (i >= stripOnePixels.numPixels() + stripTwoPixels.numPixels()) {
//                if (currLap == laps - 1) continue;


//                stripOnePixels.setPixelColor(i - (stripOnePixels.numPixels() + stripTwoPixels.numPixels()), 0, 0, 0);
                } else if (i >= stripOnePixels.numPixels()) {
                    int x = i - stripOnePixels.numPixels();


                    stripTwoPixels.setPixelColor(stripTwoPixels.numPixels() - x, 0, 0, 0);


                } else {
                    stripOnePixels.setPixelColor(i, 0, 0, 0);
                }
            }
        }

        for (uint16_t i = pulse->getOffset(); i < pulse->getOffset() + pulse->getLength(); i++) {
            if (!on) continue;

            uint8_t w = stop ? (uint8_t) (255 * prevF) : (uint8_t) (255 *
                                                                    (((float) i - (float) pulse->getOffset()) /
                                                                     (float) pulse->getLength()));
//        Serial.println(w, DEC);
            if (i >= stripOnePixels.numPixels() + stripTwoPixels.numPixels()) {
//                if (!hasSubtracted) {
//                    hasSubtracted = true;
//                    currIndex -= pulse->getLength();
//                }

//                if (currLap == 1) continue;

//                stripOnePixels.setPixelColor(i - (stripOnePixels.numPixels() + stripTwoPixels.numPixels()),
//                                             stripOnePixels.Color(prevRed, prevGreen, prevBlue, w));
            } else if (i >= stripOnePixels.numPixels()) {
                int x = i - stripOnePixels.numPixels();
                if (i == currIndex) {
//                    stripTwoPixels.setPixelColor(stripTwoPixels.numPixels() - x,
//                                                 stripOnePixels.Color(prevRed, prevGreen, prevBlue, 0));
                    if (currIndex >= 0)
                        currIndex -= pulse->getLength();
                } else {
                    stripTwoPixels.setPixelColor(stripTwoPixels.numPixels() - x,
                                                 stripOnePixels.Color(pulse->getR(), pulse->getG(), pulse->getB(), w));
                }
//            Serial.println(stripTwoPixels.numPixels() - x, DEC);

            } else {
//                if (currIndex < 120)
//                Serial.println(i, DEC);
                if (i == currIndex) {
//                    stripOnePixels.setPixelColor(i, stripOnePixels.Color(prevRed, prevGreen, prevBlue, 0));
                    currIndex -= pulse->getLength();
                } else {
                    stripOnePixels.setPixelColor(i, stripOnePixels.Color(pulse->getR(), pulse->getG(), pulse->getB(), w));
                }


//        stripOnePixels.show();
            }
//        stripOnePixels.show();
        }

        if (currIndex <= 0) {
            stop = true;
            currIndex = 0;
        }

//        if (pulse->getLength() < 8)
//            pulse->setLength(pulse->getLength() + 1);
    }

//    for (uint16_t i = lightOffset; i < lightOffset + bulletLength; i++) {
//        if (!on) continue;
//
//        uint8_t w = (uint8_t) (255 * (((float) i - (float) lightOffset) / (float) bulletLength));
////        Serial.println(w, DEC);
//        if (i >= stripOnePixels.numPixels() + stripTwoPixels.numPixels()) {
//            if (currLap == laps - 1) continue;
//
//            stripTwoPixels.setPixelColor(i - (stripOnePixels.numPixels() + stripTwoPixels.numPixels()),
//                                         stripOnePixels.Color(prevRed, prevGreen, prevBlue, w));
//        } else if (i >= stripTwoPixels.numPixels()) {
//            int x = i - stripTwoPixels.numPixels();
////            Serial.println(stripTwoPixels.numPixels() - x, DEC);
//            stripOnePixels.setPixelColor(stripOnePixels.numPixels() - x,
//                                         stripTwoPixels.Color(prevRed, prevGreen, prevBlue, w));
//        } else {
//            stripTwoPixels.setPixelColor(i, stripOnePixels.Color(prevRed, prevGreen, prevBlue, w));
//
////        stripOnePixels.show();
//        }
////        stripOnePixels.show();
//    }

//    delayMicroseconds(1);
//    delay((uint16_t) round(5 * (1 - ((float) currLap / (float) laps))));
//    delay((uint16_t) round(
//            50 * (1 - ((float) lightOffset / (float) (stripOnePixels.numPixels() + stripTwoPixels.numPixels())))));

    bool allStopped = true;
    for (Pulse *pulse : pulses) {
        if (pulse->isStopped()) continue;
//        Serial.println(pulse->getTimer()->left_ms(), DEC);
        if (!pulse->hasFired() && timer->left_ms() == 0) {
            pulse->fire();
            timer->countdown_ms(200);
            break;
        } else if (pulse->hasFired()) {
            if (pulse->getOffset() >= stripOnePixels.numPixels() + stripTwoPixels.numPixels()) {
                currLap++;
                prevF = (float) 1.0;
                pulse->stop();
                pulse->setOffset(0);
            } else {
                allStopped = false;
                pulse->setOffset(pulse->getOffset() + 1);
            }
        }
    }

    if (allStopped) stop = true;

    stripOnePixels.show();
    stripTwoPixels.show();
}

void setup() {
    Serial.begin(9600);
    pinMode(STRIP_ONE_PIN, OUTPUT);
    pinMode(STRIP_TWO_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    stripOnePixels.begin();
    stripTwoPixels.begin();
    colorWipe(true, true);
    for (uint8_t j = 0; j < sizeof(pulses) / sizeof(pulses[0]); j++) {
//        Serial.println(x, DEC);
        pulses[j] = new Pulse(pulseLength, (uint8_t) (rand() % 255), (uint8_t) (rand() % 255), (uint8_t) (rand() % 255));
//        pulses[j] = new Pulse(pulseLength, prevRed, prevGreen, prevBlue);
//        pulses[j] = new Pulse(pulseLength, 36, 0, 243);
    }

    timer = new ArduinoTimer(200);
    pulses[0]->fire();
//    pulses[0]->fire();
}

void loop() {
    if (on) {
        bulletIsh();
    }
    buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH && prevButtonState == LOW) {
        delay(20);
        buttonState = digitalRead(BUTTON_PIN);
        if (buttonState == HIGH && prevButtonState == LOW) {
            on = !on;
            colorWipe(false, true);

            if (on) {
                for (Pulse *pulse : pulses) {
                    pulse->reset();
                }
                timer->countdown_ms(200);
                pulses[0]->fire();
                prevF = 0.0;
                prevF2 = 1.0;
                prevF3 = 0.0;
                stop = false;
                currIndex = (stripOnePixels.numPixels() + stripTwoPixels.numPixels()) - 1;
                currLap = 0;
            }
        }
    }

    prevButtonState = buttonState;
}