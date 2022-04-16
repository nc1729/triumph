#pragma once

#include <cstdint>

struct Pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Pixel() : red{0}, green{0}, blue{0} {};
    Pixel(uint8_t red, uint8_t green, uint8_t blue) :
        red{red}, green{green}, blue{blue} {};
};