#pragma once

#include <iostream>

#include "Tryte.h"

/*
Device class
- General base class for devices that communicate with TRIUMPH via its data ports.
*/
class Device
{

public:
    virtual Device& operator<<(Tryte const& tryte);
    virtual Device& operator>>(Tryte& tryte);
};