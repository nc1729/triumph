#pragma once

#include "CLOptions.h"
#include "Tryte.h"

int tas(CLOptions const& options);

Tryte const fetch_address(std::string const& tryte_string);