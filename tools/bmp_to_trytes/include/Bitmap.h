#pragma once

#include "Pixel.h"

#include <vector>
#include <string>
#include <cstdint>

class Bitmap
{
private:
    // internal storage for pixels of bitmap
    std::vector<Pixel> pixels_;
    // number of rows of pixels
    uint16_t height_;
    // number of columns of pixels
    uint16_t width_;

    // helper function to extract u16 from byte array
    uint16_t static read_u16(uint8_t* const start_byte);
    // helper function to extract u32 from byte array
    uint32_t static read_u32(uint8_t* const start_byte);

public:
    Bitmap() = delete;
    // load bitmap from file
    Bitmap(std::string const& filename);
    Pixel& operator()(size_t const& i, size_t const& j);
    Pixel const& operator()(size_t const& i, size_t const& j) const;

    Pixel& operator[](size_t const& i);
    Pixel const& operator[](size_t const& i) const;

    // get height
    uint16_t height() const;
    // get width
    uint16_t width() const;
};