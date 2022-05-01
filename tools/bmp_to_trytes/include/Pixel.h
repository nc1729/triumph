#pragma once

#include <cstdint>
#include <ostream>

struct Pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Pixel() : red{0}, green{0}, blue{0} {};
    Pixel(uint8_t red, uint8_t green, uint8_t blue) :
        red{red}, green{green}, blue{blue} {};
    friend std::ostream& operator<<(std::ostream& out, Pixel const& pixel)
    {
        out << static_cast<uint16_t>(pixel.red) << ", ";
        out << static_cast<uint16_t>(pixel.green) << ", ";
        out << static_cast<uint16_t>(pixel.blue);
        return out;
    }

    bool operator==(Pixel const& other) const
    {
        return this->red == other.red && this->green == other.green && this->blue == other.blue;
    }
    
    bool operator!=(Pixel const& other) const
    {
        return !(*this == other);
    }
};

namespace PixelColors
{
    const Pixel BLACK(0, 0, 0);
    const Pixel WHITE(255, 255, 255);
    const Pixel RED(255, 0, 0);
    const Pixel GREEN(0, 255, 0);
    const Pixel BLUE(0, 0, 255);
}