#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <array>
#include <algorithm>

#include "constants.h"
#include "Tryte.h"

std::map<char, int64_t> const Tryte::schar_to_val =
	{ {'M', -13}, {'L', -12}, {'K', -11}, {'J', -10}, {'I', -9}, {'H', -8}, {'G', -7}, {'F', -6}, {'E', -5},
	  {'D',  -4}, {'C',  -3}, {'B',  -2}, {'A',  -1}, {'0',  0}, {'a',  1}, {'b',  2}, {'c',  3}, {'d',  4},
	  {'e',   5}, {'f',   6}, {'g',   7}, {'h',   8}, {'i',  9}, {'j', 10}, {'k', 11}, {'l', 12}, {'m', 13} };
std::string const Tryte::schars = "MLKJIHGFEDCBA0abcdefghijklm";

// default constructor
Tryte::Tryte()
{
	// initialise internal trit array with zeroes
	trits_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

// integer constructor
Tryte::Tryte(int64_t n)
{
	// fill internal trit array
	for (size_t i = 0; i < 9; i++)
	{
		int8_t rem = static_cast<int8_t>(n % 3);
		n /= 3;
		// deal with carry
		if (rem == 2)
		{
			rem = -1;
			n++;
		}
		else if (rem == -2)
		{
			rem = 1;
			n--;
		}
		trits_[i] = rem;
	}
}

// string constructor
Tryte::Tryte(std::string const& s)
{
	if (s.find_first_not_of(constants::septavingt_chars) == std::string::npos && s.length() == 3)
	{
		// s was a valid Tryte string
		int64_t n = 729 * schar_to_val.at(s[0]) + 27 * schar_to_val.at(s[1]) + schar_to_val.at(s[2]);
		// fill internal trit array
		for (size_t i = 0; i < 9; i++)
		{
			int8_t rem = static_cast<int8_t>(n % 3);
			n /= 3;
			// deal with carry
			if (rem == 2)
			{
				rem = -1;
				n++;
			}
			else if (rem == -2)
			{
				rem = 1;
				n--;
			}
			trits_[i] = rem;
		}
	}
	else
	{
		std::string err_str = s + " is not a valid Tryte";
		throw std::invalid_argument(err_str);
	}
}
/*
Tryte(std::string const& s)
{
	if 
}
		//
	//{};
*/

// ternary array constructor
Tryte::Tryte(std::array<int8_t, 9> const& arr)
{
	trits_ = arr;
}

// get_high - get the value (as an 8-bit integer taking the values [-13, 13]) of the high three trits
int8_t Tryte::get_high(Tryte const& tryte)
{
	return tryte[6] + 3 * tryte[7] + 9 * tryte[8];
}
// get_mid - get the value (as an 8-bit integer taking the values [-13, 13]) of the middle three trits
int8_t Tryte::get_mid(Tryte const& tryte)
{
	return tryte[3] + 3 * tryte[4] + 9 * tryte[5];
}
// get_low - get the value (as an 8-bit integer taking the values [-13, 13]) of the low three trits
int8_t Tryte::get_low(Tryte const& tryte)
{
	return tryte[0] + 3 * tryte[1] + 9 * tryte[2];
}

// get_str - get the Tryte as a septavingt string (three chars from [A-M]0[a-m])
std::string Tryte::get_str(Tryte const& tryte)
{
	std::string out(3, '0');
	out[0] = Tryte::schars[Tryte::get_high(tryte) + 13];
	out[1] = Tryte::schars[Tryte::get_mid(tryte) + 13];
	out[2] = Tryte::schars[Tryte::get_low(tryte) + 13];
	return out;
}

// access to trits
int8_t& Tryte::operator[](size_t const n)
{
	return this->trits_[n];
}

// const access to trits
int8_t const& Tryte::operator[](size_t const n) const
{
	return this->trits_[n];
}

// increment operator
Tryte& Tryte::operator+=(int64_t const n)
{
	Tryte::add_with_carry(*this, Tryte(n), 0);
	return *this;
}
// decrement operator
Tryte& Tryte::operator-=(int64_t const n)
{
	Tryte::add_with_carry(*this, Tryte(-n), 0);
	return *this;
}
/*
relational operators
*/

bool Tryte::operator==(Tryte const& other) const
{
	return this->trits_ == other.trits_;
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
		out_array[i] = std::min(this->trits_[i], other.trits_[i]);
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
		out_array[i] = std::max(this->trits_[i], other.trits_[i]);
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
		out_array[i] = -this->trits_[i];
	}
	return Tryte(out_array);
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

	int64_t high = Tryte::schar_to_val.at(c[0]);
	int64_t mid = Tryte::schar_to_val.at(c[1]);
	int64_t low = Tryte::schar_to_val.at(c[2]);

	tryte = Tryte(729 * high + 27 * mid + low);

	return is;
}

void Tryte::add_with_carry(Tryte& t1, Tryte const& t2, int8_t& carry)
{
	int8_t temp = 0;

	// fill t1 with result of t1 + t2
	for (size_t i = 0; i < 9; i++)
	{
		temp = t1[i] + t2[i] + carry;
		carry = 0;
		if (temp > 1)
		{
			temp -= 3;
			carry += 1;
		}
		else if (temp < -1)
		{
			temp += 3;
			carry -= 1;
		}
		t1[i] = temp;
	}
}

void Tryte::add_with_carry(Tryte& t1, Tryte const& t2, int8_t&& carry)
{
	Tryte::add_with_carry(t1, t2, carry);
}

void Tryte::subtract_with_borrow(Tryte& t1, Tryte const& t2, int8_t& borrow)
{
	int8_t temp = 0;

	// fill t1 with result of t1 - t2
	for (size_t i = 0; i < 9; i++)
	{
		temp = t1[i] - t2[i] + borrow;
		borrow = 0;
		if (temp > 1)
		{
			temp -= 3;
			borrow += 1;
		}
		else if (temp < -1)
		{
			temp += 3;
			borrow -= 1;
		}
		t1[i] = temp;
	}
}

void Tryte::star(Tryte& t1, Tryte const& t2)
{
	// fill t1 with result of t1 * t2 (tritwise multiplication)
	for (size_t i = 0; i < 9; i++)
	{
		t1[i] *= t2[i];
	}
}

void Tryte::shift(Tryte& t1, Tryte const& t2)
{
	int8_t shift = Tryte::get_low(t2);

	if (shift >= 9)
	{
		// shift out of range - overflow
		t1 = 0;
	}
	else if (shift <= -9)
	{
		t1 = 0;
	}
	else if (shift == 0)
	{
		// shift by nothing, do nothing
	}
	else if (shift > 0)
	{
		// tritshift left
		for (int8_t i = 0; shift + i < 9; i++)
		{
			t1[i] = t1[shift + i];
		}
	}
	else
	{
		// tritshift right - work backwards to avoid overwriting trits needed later
		for (int8_t i = 8; i >= 0; i--)
		{
			if (shift + i >= 0)
			{
				t1[i] = t1[shift + i];
			}
			else
			{
				t1[i] = 0;
			}
		}
	}
}

void Tryte::flip(Tryte& t1)
{
	for (size_t i = 0; i < 9; i++)
	{
		t1[i] = -t1[i];
	}
}
