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
	_high = static_cast<int8_t>(n / 729);

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
	_mid = static_cast<int8_t>(n / 27);

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
	_low = static_cast<int8_t>(n);
}

Tryte::Tryte(std::string const& s)
{
	_high = schar_to_val.at(s[0]);
	_mid = schar_to_val.at(s[1]);
	_low = schar_to_val.at(s[2]);
}

Tryte::Tryte(std::array<int8_t, 9> const& arr)
{
	_high = (9 * arr[0]) + (3 * arr[1]) + arr[2];
	_mid  = (9 * arr[3]) + (3 * arr[4]) + arr[5];
	_low  = (9 * arr[6]) + (3 * arr[7]) + arr[8];
}

std::string Tryte::get_str(Tryte const& tryte)
{
	std::string out(3, '0');
	out[0] = Tryte::schars[tryte._high + 13];
	out[1] = Tryte::schars[tryte._mid + 13];
	out[2] = Tryte::schars[tryte._low + 13];
	return out;
}

int8_t Tryte::sign(Tryte const& tryte)
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

std::array<int8_t, 9> Tryte::ternary_array(Tryte const& tryte)
{
	std::array<int8_t, 9> output;

	int64_t const tryte_int = Tryte::get_int(tryte);
	// store sign, algorithm is easier if everything is positive
	int8_t const sign = Tryte::sign(tryte);
	int64_t dividend = sign > 0 ? tryte_int : -tryte_int;
	int64_t remainder = 0;

	for (size_t i = 0; i < 9; i++)
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
		output[8 - i] = static_cast<int8_t>(remainder);
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
	return Tryte::add_with_carry(*this, other, 0);
}

Tryte& Tryte::operator+=(Tryte const& other)
{
	*this = *this + other;
	return *this;
}

Tryte Tryte::operator-(Tryte const& other) const
{
	return (*this) + (-other);
}

Tryte& Tryte::operator-=(Tryte const& other)
{
	*this = *this - other;
	return *this;
}

Tryte Tryte::operator-() const
{
	int64_t const flip = -Tryte::get_int(*this);
	return Tryte(flip);
}

std::ostream& operator<<(std::ostream& os, Tryte const& tryte)
{
	os << Tryte::get_str(tryte);
	return os;
}

std::istream& operator>>(std::istream& is, Tryte& tryte)
{
	char c[3] = {0, 0, 0};

	is >> c[0] >> c[1] >> c[2];

	tryte._high = Tryte::schar_to_val.at(c[0]);
	tryte._mid = Tryte::schar_to_val.at(c[1]);
	tryte._low = Tryte::schar_to_val.at(c[2]);

	return is;
}

Tryte Tryte::add_with_carry(Tryte const& t1, Tryte const& t2, int8_t& carry)
{
	int8_t temp;

	int8_t out_high = 0;
	int8_t out_mid = 0;
	int8_t out_low = 0;

	temp = t1._low + t2._low + carry;
	carry = 0;
	if (temp > 13)
	{
		temp -= 27;
		carry += 1;
	}
	else if (temp < -13)
	{
		temp += 27;
		carry -= 1;
	}
	out_low = temp;

	temp = t1._mid + t2._mid + carry;
	carry = 0;
	if (temp > 13)
	{
		temp -= 27;
		carry += 1;
	}
	else if (temp < -13)
	{
		temp += 27;
		carry -= 1;
	}
	out_mid = temp;

	temp = t1._high + t2._high + carry;
	carry = 0;
	if (temp > 13)
	{
		temp -= 27;
		carry += 1;
	}
	else if (temp < -13)
	{
		temp += 27;
		carry -= 1;
	}
	out_high = temp;

	return Tryte(out_high, out_mid, out_low);
}

Tryte Tryte::add_with_carry(Tryte const& t1, Tryte const& t2, int8_t&& carry)
{
	return Tryte::add_with_carry(t1, t2, carry);
}

Tryte Tryte::shift(Tryte const& t1, Tryte const& t2)
{
	int8_t shift = t2._low;

	if (shift >= 9 || shift <= -9)
	{
		// shift out of range
		return 0;
	}
	else if (shift == 0)
	{
		// shift by nothing, do nothing
		return t1;
	}
	else if (shift > 0)
	{
		// tritshift left
		std::array<int8_t, 9> t1_array = Tryte::ternary_array(t1);

		for (int8_t i = 0; shift + i < 9; i++)
		{
			t1_array[i] = t1_array[shift + i];
		}
		return Tryte(t1_array);
	}
	else
	{
		// tritshift right
		std::array<int8_t, 9> t1_array = Tryte::ternary_array(t1);

		for (int8_t i = 8; i >= 0; i--)
		{
			if (shift + i >= 0)
			{
				t1_array[i] = t1_array[shift + i];
			}
			else
			{
				t1_array[i] = 0;
			}
		}
		return Tryte(t1_array);
	}
}