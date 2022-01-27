#include <iostream>

#include "Device.h"
#include "Tryte.h"

void Device::raw_mode()
{
    _mode = Mode::RAW;
}

void Device::num_mode()
{
    _mode = Mode::NUM;
}

Device& Device::operator<<(Tryte const& tryte)
{
    if (_mode == Mode::RAW)
    {
        *_output << tryte;
    }
    else if (_mode == Mode::NUM)
    {
        *_output << Tryte::get_int(tryte);
    }
    return (*this);
}

Device& Device::operator>>(Tryte& tryte)
{
    *_input >> tryte;
    return (*this);
}