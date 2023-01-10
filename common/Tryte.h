#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <array>
#include <functional> // for hash specialisation

class Tryte
{
private:
	// trits arranged from least significant to most significant
	std::array<int8_t, 9> trits_{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	static std::map<char, int64_t> const schar_to_val;
	static std::string const schars;

public:
	/*
	constructors
	*/

	// default constructor (default value is 0)
	Tryte() {};

	// integer constructor
	constexpr Tryte(int64_t n)
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

			// trits are zero by default - if n == 0 break out of loop early
			if (n == 0) break;
		}
	}

	// string constructor (uses integer constructor)
	Tryte(std::string const& s);

	// ternary array constructor
	Tryte(std::array<int8_t, 9> const& arr);

	// ternary array rvalue constructor
	Tryte(std::array<int8_t, 9> const&& arr);

	/*
	useful functions
	*/
	int64_t static constexpr get_int(Tryte const& tryte)
	{
		int64_t result = 0;
		int64_t power_of_3 = 1;
		for (size_t i = 0; i < 9; i++)
		{
			result += (power_of_3 * static_cast<int64_t>(tryte[i]));
			power_of_3 *= 3;
		}
		return result;
	};

	// get_high - get the value (as an 8-bit integer taking the values [-13, 13]) of the high three trits
	int8_t static get_high(Tryte const& tryte);
	// get_mid - get the value (as an 8-bit integer taking the values [-13, 13]) of the middle three trits
	int8_t static get_mid(Tryte const& tryte);
	// get_low - get the value (as an 8-bit integer taking the values [-13, 13]) of the low three trits
	int8_t static get_low(Tryte const& tryte);

	// get_str - get the Tryte as a septavingt string (three chars from [A-M]0[a-m])
	std::string static get_str(Tryte const& tryte);
	// sign - get the sign of the Tryte
	int8_t static constexpr sign(Tryte const& tryte)
	{
		// loop through trits, starting with most significant
		// if a non-zero trit is found, return its value
		for (size_t i = 0; i < 9; i++)
		{
			if (tryte[8 - i] != 0)
			{
				return tryte[8 - i];
			}
		}
		// if all trits are zero, tryte is zero, so its sign is zero
		return 0;
	}

	/*
	cast to int64_t
	*/
	operator int64_t() const { return Tryte::get_int(*this); }
	
	/*
	quick access to trits
	*/
	int8_t& operator[](size_t const n);
	int8_t const& operator[](size_t const n) const;

	/*
	increment/decrement operators
	*/
    Tryte& operator+=(Tryte const& n);
	Tryte& operator-=(Tryte const& n);
	Tryte& operator++();
	Tryte& operator--();

	/*
	relational operators
	*/
	bool operator==(Tryte const& other) const;
	bool operator!=(Tryte const& other) const;
	bool operator<(Tryte const& other) const;
	bool operator<=(Tryte const& other) const;
	bool operator>(Tryte const& other) const;
	bool operator>=(Tryte const& other) const;

	/*
	tritwise logical operators
	*/
	// ternary AND operator
	Tryte operator&(Tryte const& other) const;
	Tryte& operator&=(Tryte const& other);
	// ternary OR operator
	Tryte operator|(Tryte const& other) const;
	Tryte& operator|=(Tryte const& other);
	// ternary NOT operator
	Tryte operator~() const;

	/*
	stream extraction and insertion operators
	*/
	friend std::ostream& operator<<(std::ostream& os, Tryte const& tryte);
	friend std::istream& operator>>(std::istream& is, Tryte& tryte);

	/*
	arithmetic operators
	*/
	// addition that respects the carry flag
	static void add_with_carry(Tryte& t1, Tryte const& t2, int8_t& carry);
	static void add_with_carry(Tryte& t1, Tryte const& t2, int8_t&& carry);
	// subtraction that respects the carry flag
	static void subtract_with_borrow(Tryte& t1, Tryte const& t2, int8_t& borrow);
	// star operator (tritwise product of two Trytes)
	static void star(Tryte& t1, Tryte const& t2);
	// shift tritwise
	static void shift(Tryte& t1, Tryte const& t2);
	// flip sign
	static void flip(Tryte& t1);

};

// define hash function for Trytes, for use with unordered_map
// it's sufficient just to use the unique integer associated with each Tryte value
// this guarantees no collisions
template<>
struct std::hash<Tryte>
{
	size_t operator()(Tryte const& t) const
	{
		return Tryte::get_int(t);
	}
};
