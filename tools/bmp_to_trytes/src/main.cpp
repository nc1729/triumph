#include "Bitmap.h"
#include "Pixel.h"
#include "Tryte.h"

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <stdexcept>

int8_t pixel_to_trit(Pixel const& pixel)
{
    if (pixel == PixelColors::BLACK)
    {
        return -1;
    }
    else if (pixel == PixelColors::WHITE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

std::vector<Tryte> bmp_to_tiles(Bitmap const& bmp)
{
    if (bmp.height() % 9 != 0 || bmp.width() % 9 != 0)
    {
        throw std::runtime_error("Invalid bitmap for conversion to tilemap- height and width must be divisible by 9");
    }
    std::vector<Tryte> tile_trytes;
    uint16_t tile_height = bmp.height() / 9;
    uint16_t tile_width = bmp.width() / 9;
    // loop over tile rows
    for (size_t i = 0; i < tile_height; i++)
    {
        // loop over tiles
        for (size_t j = 0; j < tile_width; j++)
        {
            // loop over pixels in tile, from top to bottom
            for (size_t off_y = 0; off_y < 9; off_y++)
            {
                std::array<int8_t, 9> row_trits;
                for (size_t off_x = 0; off_x < 9; off_x++)
                {
                    row_trits[off_x] = pixel_to_trit(bmp(9 * j + off_y, 9 * i + off_x));
                }
                // trits are stored 'right to left' in Trytes - tryte[0] is least significant digit
                // (rightmost digit as a number, but leftmost on the bitmap!)
                tile_trytes.push_back(row_trits);
            }
        }
    }

    // tiles are 9 Trytes long, stored from the tilemap left-to-right, top-to-bottom
    return tile_trytes;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: bmp_to_trytes [FILENAME]\n";
        return 1;
    }
    std::string filename = argv[1];
    Bitmap bmp(filename);

    try
    {
        std::vector<Tryte> trytes = bmp_to_tiles(bmp);
        for (auto const& tryte : trytes)
        {
            std::cout << tryte;
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}