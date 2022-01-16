#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <array>

#include "Tryte.h"

Tryte::Tryte(int64_t n)
{
	// calculate _high
	n %= 19683;
	if (n > 9841)
	{
		n -= 19683;
	}
	else if (n < -9841)
	{
		n += 19683;
	}
	_high = n / 729;

	// calculate _mid
	n %= 729;
	if (n > 364)
	{
		n -= 729;
		// carry to high
		_high++;
	}
	else if (n < -364)
	{
		n += 729;
		// borrow from high
		_high--;
	}
	_mid = n / 27;

	// calculate _low
	n %= 27;
	if (n > 13)
	{
		n -= 27;
		_mid++;
	}
	else if (n < -13)
	{
		n += 27;
		_mid--;
	}
	_low = n;
}

Tryte::Tryte(char[4] const& s)
{
	try
	{
		_high = schar_to_val.at(s[0]);
		_mid  = schar_to_val.at(s[1]);
		_low  = schar_to_val.at(s[2]);
	}
	catch (std::out_of_range& e)
	{
		throw std::out_of_range("Invalid string for Tryte initialisation.");
	}
}

int64_t constexpr Tryte::get_int(Tryte const& tryte)
{
	return 729 * static_cast<int64_t>(tryte._high) + 27 * static_cast<int64_t>(tryte._mid) + static_cast<int64_t>(tryte._low);
}

std::string constexpr Tryte::get_str(Tryte const& tryte)
{
	std::string out(3, '0');
	out[0] = Tryte::schars[tryte._high + 13];
	out[1] = Tryte::schars[tryte._mid + 13];
	out[2] = Tryte::schars[tryte._low + 13];
	return out;
}

int8_t constexpr Tryte::sign(Tryte const& tryte)
{
	int64_t tryte_int = Tryte::get_int(tryte);
	if (tryte_int > 0)
	{
		return 1;
	}
	else if (tryte_int < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

std::array<int8_t, 9> constexpr Tryte::ternary_array(Tryte const& tryte)
{
	std::array<int8_t, 9> output;

	int64_t const tryte_int = Tryte::get_int(tryte);
	// store sign, algorithm is easier if everything is positive
	int8_t const sign = Tryte::sign(tryte);
	int64_t dividend = sign > 0 ? tryte_int : -tryte_int;
	int64_t remainder = 0;

	for (size_t i = 8; i >= 0; i--)
	{
		remainder = dividend % 3;
		dividend /= 3;
		// handle shift for balanced ternary representation
		if (remainder == 2)
		{
			dividend++;
			remainder = -1;
		}
		// fill output array
		output[i] = remainder;
	}

	// flip sign back if necessary
	if (sign < 0)
	{
		for (auto& trit : output) trit = -trit;
	}

	return output;
}