//
// Created by jesper on 2017-01-29.
//

#include "Pulse.h"

uint16_t Pulse::getOffset() {
    return offset;
}

void Pulse::setOffset(uint16_t offset) {
    this->offset = offset;
}

void Pulse::fire() {
    fired = true;
}

bool Pulse::hasFired() {
    return fired;
}

uint8_t Pulse::getLength() {
    return length;
}

void Pulse::setLength(uint8_t length) {
    this->length = length;
}

void Pulse::stop() {
    offset = 0;
    stopped = true;
}

bool Pulse::isStopped() {
    return stopped;
}

bool Pulse::isFinalized() {
    return finalized;
}

void Pulse::finalize() {
    finalized = true;
}

void Pulse::reset() {
    this->offset = 0;
    this->fired = false;
    this->length = length;
    this->stopped = false;
    this->finalized = false;
}

uint8_t Pulse::getR() {
    return red;
}

uint8_t Pulse::getG() {
    return green;
}

uint8_t Pulse::getB() {
    return blue;
}

void Pulse::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    this->red = r;
    this->green = g;
    this->blue = b;
}
