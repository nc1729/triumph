#pragma once

#include <iostream>

#include "Tryte.h"

enum class Mode
{
    RAW,
    NUM
};

/*
Device class
- General base class for devices that communicate with TRIUMPH via its data ports.
*/
class Device
{
private:
    std::istream* _input;
    std::ostream* _output;
    Mode _mode;

public:
    Device(std::istream* input, std::ostream* output) : _input{input}, _output{output}, _mode{Mode::RAW} {};

    void raw_mode();
    void num_mode();

    virtual Device& operator<<(Tryte const& tryte);
    virtual Device& operator>>(Tryte& tryte);
};