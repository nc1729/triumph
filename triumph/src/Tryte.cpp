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
	// fill internal trit array
	for (size_t i = 0; i < 9; i++)
	{
		int8_t rem = static_cast<int8_t>(n % 3);
		// deal with carry
		if (rem == 2)
		{
			rem = -1;
			n += (n / 3);
		}
		else if (rem == -2)
		{
			rem = 1;
			n -= (n / 3);
		}
		n /= 3;
		_trits[i] = rem;
	}
}

Tryte::Tryte(std::string const& s)
{
	int64_t tryte_int = 729 * schar_to_val.at(s[0]) + 27 * schar_to_val.at(s[1]) + schar_to_val.at(s[2]);
	for (size_t i = 0; i < 9; i++)
	{
		int8_t rem = static_cast<int8_t>(tryte_int % 3);
		// deal with carry
		if (rem == 2)
		{
			rem = -1;
			tryte_int += (tryte_int / 3);
		}
		else if (rem == -2)
		{
			rem = 1;
			tryte_int -= (tryte_int / 3);
		}
		tryte_int /= 3;
		_trits[i] = rem;
	}
}

Tryte::Tryte(std::array<int8_t, 9> const& arr)
{
	_trits = arr;
}

int8_t Tryte::get_high(Tryte const& tryte)
{
	return tryte[6] + 3 * tryte[7] + 9 * tryte[8];
}

int8_t Tryte::get_mid(Tryte const& tryte)
{
	return tryte[3] + 3 * tryte[4] + 9 * tryte[5];
}

int8_t Tryte::get_low(Tryte const& tryte)
{
	return tryte[0] + 3 * tryte[1] + 9 * tryte[2];
}

std::string Tryte::get_str(Tryte const& tryte)
{
	std::string out(3, '0');
	std::array<int8_t, 9> const& arr = tryte._trits;
	out[0] = Tryte::schars[Tryte::get_high(tryte) + 13];
	out[1] = Tryte::schars[Tryte::get_mid(tryte) + 13];
	out[2] = Tryte::schars[Tryte::get_low(tryte) + 13];
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

int8_t& Tryte::operator[](size_t const n)
{
	return this->_trits[n];
}

int8_t const& Tryte::operator[](size_t const n) const
{
	return this->_trits[n];
}

bool Tryte::operator==(Tryte const& other) const
{
	for (size_t i = 0; i < 9; i++)
	{
		if (this->_trits[i] != other._trits[i])
		{
			return false;
		}
	}
	return true;
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
	std::array<int8_t, 9> out_array;
	for (size_t i = 0; i < 9; i++)
	{
		out_array[i] = std::min(this->_trits[i], other._trits[i]);
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
	std::array<int8_t, 9> out_array;
	for (size_t i = 0; i < 9; i++)
	{
		out_array[i] = std::max(this->_trits[i], other._trits[i]);
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
	std::array<int8_t, 9> out_array;
	for (size_t i = 0; i < 9; i++)
	{
		out_array[i] = -this->_trits[i];
	}
	return Tryte(out_array);
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
	return ~(*this);
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