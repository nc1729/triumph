#include "Bitmap.h"
#include "Pixel.h"

#include <fstream>
#include <iterator>
#include <iostream>

uint16_t Bitmap::read_u16(uint8_t* const start_byte)
{
    // this only works if data is little-endian!
    return static_cast<uint16_t>(start_byte[0])
    + (static_cast<uint16_t>(start_byte[1]) << 8);
}

uint32_t Bitmap::read_u32(uint8_t* const start_byte)
{
    // this only works if data is little-endian!
    return static_cast<uint32_t>(start_byte[0])
    + (static_cast<uint32_t>(start_byte[1]) << 8)
    + (static_cast<uint32_t>(start_byte[2]) << 16)
    + (static_cast<uint32_t>(start_byte[3]) << 24);
}

Bitmap::Bitmap(std::string const& filename)
{
    std::ifstream bmp_file(filename);

    // read bytes from file into bytes vector
    std::vector<uint8_t> bytes(std::istreambuf_iterator<char>(bmp_file), {});

    // find starting point for pixel array in bmp file
    uint32_t pixel_offset = read_u32(&bytes[10]);

    // read height and width of bitmap file
    height_ = read_u32(&bytes[18]);
    width_ = read_u32(&bytes[22]);

    // calculate number of 'padding' bytes (each row is padded to a multiple of 4 bytes)
    uint32_t padding = width_ % 4;

    // read pixel data from file
    pixels_.resize(height_ * width_);
    uint64_t pos = pixel_offset;
    for (uint32_t i = 0; i < height_; i++)
    {
        // read each row into Pixel vector, from bottom to top
        for (uint32_t j = 0; j < width_; j++)
        {
            pixels_[width_ * i + j].blue = bytes[pos];
            pixels_[width_ * i + j].green = bytes[pos + 1];
            pixels_[width_ * i + j].red = bytes[pos + 2];
            pos += 3; // move to next pixel
        }
        // skip the padding bytes!
        pos += padding;
    }
}

Pixel& Bitmap::operator()(size_t const& i, size_t const& j)
{
    // Pixels are stored from bottom row to top - correct for this
    return pixels_[width_ * (height_ - i - 1) + j];
}

Pixel const& Bitmap::operator()(size_t const& i, size_t const& j) const
{
    return pixels_[width_ * (height_ - i - 1) + j];
}

Pixel& Bitmap::operator[](size_t const& i)
{
    return pixels_[i];
}

Pixel const& Bitmap::operator[](size_t const& i) const
{
    return pixels_[i];
}

uint16_t Bitmap::height() const
{
    return height_;
}

uint16_t Bitmap::width() const
{
    return width_;
}