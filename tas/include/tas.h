#pragma once

#include "TASCLOptions.h"
#include "common/Tryte.h"

int tas(TASCLOptions const& options);

Tryte const fetch_address(std::string const& tryte_string);