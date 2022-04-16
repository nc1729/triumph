#include "Bitmap.h"
#include "Pixel.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<uint8_t> read_bmp_file(std::string const& filename)
{
    std::fstream bmp_file(filename);
    std::vector<uint8_t> file_bytes;

    char byte;
    while (bmp_file.get(byte))
    {
        file_bytes.push_back(static_cast<uint8_t>(byte));
    }

    return file_bytes;

}
int main()
{
    //std::vector<uint8_t> file_bytes = read_bmp_file("./pixels.bmp");

    Bitmap bmp("./pixels.bmp");
}