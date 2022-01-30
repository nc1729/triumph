#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <array>

class Tryte
{
private:
	// trits arranged from least significant to most significant
	std::array<int8_t, 9> _trits;
	static std::map<char, int64_t> const schar_to_val;
	static std::string const schars;

public:
	/*
	constructors
	*/

	// default constructor (default value is 0)
	Tryte();

	// integer constructor
	Tryte(int64_t n);

	// string constructor (uses integer constructor)
	Tryte(std::string const& s) : 
		Tryte(729 * schar_to_val.at(s[0]) + 27 * schar_to_val.at(s[1]) + schar_to_val.at(s[2]))
	{};

	// ternary array constructor
	Tryte(std::array<int8_t, 9> const& arr);

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
    Tryte& operator+=(int64_t const n);
	Tryte& operator-=(int64_t const n);

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
