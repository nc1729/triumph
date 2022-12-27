#include <iostream>
#include <string>

#include "common/debug_utils.h"
#include "common/Log.h"

void Log::error(std::string const& msg)
{
    std::cout << msg << '\n';
    DEB_assert(false);
}

void Log::log(std::string const& msg)
{

}