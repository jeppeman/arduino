//
// Created by jesper on 2017-01-29.
//

#ifndef BOARD_PULSE_H
#define BOARD_PULSE_H


#include <Arduino.h>
#include <Temboo.h>
#include <utility/ArduinoTimer.h>

class Pulse {
private:
    uint8_t length;
    uint16_t offset;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    bool fired;
    bool stopped;
    bool finalized;
public:
    Pulse(unsigned int length, uint8_t red, uint8_t green, uint8_t blue) {
        this->offset = 0;
        this->fired = false;
        this->length = length;
        this->stopped = false;
        this->finalized = false;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    uint16_t getOffset();

    void setOffset(uint16_t offset);

    void fire();

    void stop();

    bool isStopped();

    bool isFinalized();

    void finalize();

    void reset();

    bool hasFired();

    uint8_t getLength();

    void setLength(uint8_t length);

    uint8_t getR();

    uint8_t getG();

    uint8_t getB();

    void setRGB(uint8_t r, uint8_t g, uint8_t b);
};


#endif //BOARD_PULSE_H
