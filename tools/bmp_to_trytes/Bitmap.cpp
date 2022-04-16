#include "Bitmap.h"
#include "Pixel.h"

#include <fstream>
#include <iostream>

uint16_t Bitmap::read_u16(char* const start_byte)
{
    // this only works if data is little-endian!
    return static_cast<int16_t>(start_byte[0])
    + static_cast<int16_t>(start_byte[1]) << 8;
}

uint32_t Bitmap::read_u32(char* const start_byte)
{
    // this only works if data is little-endian!
    return static_cast<int32_t>(start_byte[0])
    + static_cast<int32_t>(start_byte[1]) << 8
    + static_cast<int32_t>(start_byte[2]) << 16
    + static_cast<int32_t>(start_byte[3]) << 24;
}

Bitmap::Bitmap(std::string const& filename)
{
    std::ifstream bmp_file(filename);

    // get the file size
    bmp_file.seekg(0, std::ios::end);
    size_t file_size = bmp_file.tellg();
    bmp_file.seekg(0, std::ios::beg);
    std::cout << file_size << '\n';

    std::cout << "Hello?";

    // read bytes from file into bytes vector
    std::vector<char> bytes;
    bytes.reserve(file_size);
    bmp_file.read(bytes.data(), file_size);

    int i = 0;
    for (auto const& byte : bytes)
    {
        std::cout << i << ": " << static_cast<uint8_t>(byte) << '\n';
    }

    // find starting point for pixel array in bmp file
    uint32_t pixel_offset = read_u32(&bytes[10]);

    std::cout << "Pixel offset: " << pixel_offset << '\n';

    height_ = read_u16(&bytes[18]);
    width_ = read_u16(&bytes[20]);

    std::cout << "Height: " << height_ << " Width: " << width_ << '\n';

    
}

Pixel& Bitmap::operator()(size_t const& i, size_t const& j)
{
    return pixels_[width_ * i + j];
}