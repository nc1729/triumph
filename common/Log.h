#pragma once
#include <string>

namespace Log
{
    // log an error message to console and kill program
    void error(std::string const& msg);

    // print message to console
    void log(std::string const& msg);

}