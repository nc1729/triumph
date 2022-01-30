#include <iostream>

#include "Device.h"
#include "Tryte.h"


Device& Device::operator<<(Tryte const& tryte)
{
    // default behaviour sending Tryte to device - just 
    return *this;
}

Device& Device::operator>>(Tryte& tryte)
{
    // default behaviour reading Tryte from Device - just get zeroes
    tryte = 0;
    return *this;
}