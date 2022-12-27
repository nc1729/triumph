#pragma once

#include <iostream>
#include <sstream>
#include <assert.h>

#ifndef NDEBUG // if not in release mode
#define DEB_assert(x)                              \
do                                                 \
{                                                  \
    if (!(x))                                      \
    {                                              \
         std::ostringstream error_stream;          \
         error_stream << "\n DEB_assert failure\n" << "File: " << __FILE__ << " line: " << __LINE__ << '\n'; \
         std::cout << error_stream.str() << '\n';  \
         assert((x));                              \
    }                                              \
} while(0);
#else
#define DEB_assert(x) do {} while(0);
#endif