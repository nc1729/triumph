#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct Pixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Pixel() : red{0}, green{0}, blue{0} {};
    Pixel(uint8_t red, uint8_t green, uint8_t blue) :
        red{red}, green{green}, blue{blue} {};
};

class Bitmap
{
private:
    std::vector<Pixel> pixels_;
    uint8_t height_;
    uint8_t width_;
public:
    Bitmap(std::string const& filename);
    Pixel& operator()(size_t const& i, size_t const& j);
};
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
    std::vector<uint8_t> file_bytes = read_bmp_file("./pixels.bmp");

    std::cout << file_bytes.size() << '\n';
}