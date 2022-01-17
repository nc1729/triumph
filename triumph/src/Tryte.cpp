#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>

#include "Tryte.h"

std::map<char, int8_t> const Tryte::schar_to_val =
	{ {'M', -13}, {'L', -12}, {'K', -11}, {'J', -10}, {'I', -9}, {'H', -8}, {'G', -7}, {'F', -6}, {'E', -5},
	  {'D',  -4}, {'C',  -3}, {'B',  -2}, {'A',  -1}, {'0',  0}, {'a',  1}, {'b',  2}, {'c',  3}, {'d',  4},
	  {'e',   5}, {'f',   6}, {'g',   7}, {'h',   8}, {'i',  9}, {'j', 10}, {'k', 11}, {'l', 12}, {'m', 13} };
std::string const Tryte::schars = "MLKJIHGFEDCBA0abcdefghijklm";
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

Tryte::Tryte(std::string const& s)
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

Tryte::Tryte(std::array<int8_t, 3> const& arr)
{
	_high = arr[0];
	_mid  = arr[1];
	_low  = arr[2];
}

Tryte::Tryte(std::array<int8_t, 9> const& arr)
{
	_high = (9 * arr[0]) + (3 * arr[1]) + arr[2];
	_mid  = (9 * arr[3]) + (3 * arr[4]) + arr[5];
	_low  = (9 * arr[6]) + (3 * arr[7]) + arr[8];
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

bool Tryte::operator==(Tryte const& other) const
{
	return (this->_high == other._high) && (this->_mid == other._mid) && (this->_low == other._low);
}

bool Tryte::operator!=(Tryte const& other) const
{
	return !(*this == other);
}

bool Tryte::operator<(Tryte const& other) const
{
	int64_t const this_int = Tryte::get_int(*this);
	int64_t const other_int = Tryte::get_int(other);
	return this_int < other_int;
}

bool Tryte::operator<=(Tryte const& other) const
{
	int64_t const this_int = Tryte::get_int(*this);
	int64_t const other_int = Tryte::get_int(other);
	return this_int <= other_int;
}

bool Tryte::operator>(Tryte const& other) const
{
	int64_t const this_int = Tryte::get_int(*this);
	int64_t const other_int = Tryte::get_int(other);
	return this_int > other_int;
}

bool Tryte::operator>=(Tryte const& other) const
{
	int64_t const this_int = Tryte::get_int(*this);
	int64_t const other_int = Tryte::get_int(other);
	return this_int >= other_int;
}

Tryte Tryte::operator&(Tryte const& other) const
{
	std::array<int8_t, 9> const this_array = Tryte::ternary_array(*this);
	std::array<int8_t, 9> const other_array = Tryte::ternary_array(other);

	std::array<int8_t, 9> out_array;

	for (size_t i = 0; i < 9; i++)
	{
		out_array[i] = std::min(this_array[i], other_array[i]);
	}

	return Tryte(out_array);
}

Tryte& Tryte::operator&=(Tryte const& other)
{
	*this = *this & other;
	return *this;
}

Tryte Tryte::operator|(Tryte const& other) const
{
	std::array<int8_t, 9> const this_array = Tryte::ternary_array(*this);
	std::array<int8_t, 9> const other_array = Tryte::ternary_array(other);

	std::array<int8_t, 9> out_array;

	for (size_t i = 0; i < 9; i++)
	{
		out_array[i] = std::max(this_array[i], other_array[i]);
	}

	return Tryte(out_array);
}

Tryte& Tryte::operator|=(Tryte const& other)
{
	*this = *this | other;
	return *this;
}

Tryte Tryte::operator~() const
{
	int64_t const flip = -Tryte::get_int(*this);
	return Tryte(flip);
}

Tryte Tryte::operator+(Tryte const& other) const
{
	int8_t temp;
	int8_t carry = 0;
	
	temp = this->_low + other._low;
	if (temp > 13)
	{
		temp -= 13;
		carry += 1;
	}
	else if (temp < -13)
	{
		temp += 13;
		carry -= 1;
	}
}