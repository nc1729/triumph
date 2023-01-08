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

	// string constructor (uses integer constructor)
	Tryte(std::string const& s);

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
			this->trits_[i] = rem;

			// trits are zero by default - if n == 0 break out of loop early
			if (n == 0) break;
		}
	};

	// ternary array constructor
	Tryte(std::array<int8_t, 9> const& arr);

	// ternary array rvalue constructor
	Tryte(std::array<int8_t, 9> const&& arr);

	/*
	useful functions
	*/
	int64_t constexpr get_int() const
	{
		int64_t result = 0;
		int64_t power_of_3 = 1;
		for (size_t i = 0; i < 9; i++)
		{
			result += (power_of_3 * static_cast<int64_t>((*this)[i]));
			power_of_3 *= 3;
		}
		return result;
	};

	// get_high - get the value (as an 8-bit integer taking the values [-13, 13]) of the high three trits
	int8_t get_high() const
	{
		return (*this)[6] + 3 * (*this)[7] + 9 * (*this)[8];
	};
	// get_mid - get the value (as an 8-bit integer taking the values [-13, 13]) of the middle three trits
	int8_t get_mid() const
	{
		return (*this)[3] + 3 * (*this)[4] + 9 * (*this)[5];
	};
	// get_low - get the value (as an 8-bit integer taking the values [-13, 13]) of the low three trits
	int8_t get_low() const
	{
		return (*this)[0] + 3 * (*this)[1] + 9 * (*this)[2];
	};

	// get_str - get the Tryte as a septavingt string (three chars from [A-M]0[a-m])
	std::string get_str() const;

	// sign - get the sign of the Tryte
	int8_t constexpr sign()
	{
		// loop through trits, starting with most significant
		// if a non-zero trit is found, return its value
		for (size_t i = 0; i < 9; i++)
		{
			if ((*this)[8 - i] != 0)
			{
				return (*this)[8 - i];
			}
		}
		// if all trits are zero, tryte is zero, so its sign is zero
		return 0;
	}

	/*
	cast to int64_t
	*/
	//operator int64_t() const { return Tryte::get_int(*this); }
	
	/*
	quick access to trits
	*/
	int8_t& operator[](size_t const n);
	int8_t const& operator[](size_t const n) const;

	// unary minus operator
	Tryte operator-() const;

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
	// addition without carry
	friend Tryte operator+(Tryte lhs, Tryte const& rhs);
	// subtraction that respects the carry flag
	static void subtract_with_borrow(Tryte& t1, Tryte const& t2, int8_t& borrow);
	// subtraction without borrow
	friend Tryte operator-(Tryte lhs, Tryte const& rhs);
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
		return t.get_int();
	}
};
