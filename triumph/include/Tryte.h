#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <array>

class Tryte
{
private:
	std::array<int8_t, 9> _trits;
	static std::map<char, int8_t> const schar_to_val;
	static std::string const schars;

public:
	/*
	constructors
	*/

	// default constructor (default value is 0)
	Tryte();

	// integer constructor
	Tryte(int64_t n);

	// string constructor
	Tryte(std::string const& s);

	// septavingt array constructor
	Tryte(int8_t high, int8_t mid, int8_t low);

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
	};
	int8_t static get_high(Tryte const& tryte);
	int8_t static get_mid(Tryte const& tryte);
	int8_t static get_low(Tryte const& tryte);
	std::string static get_str(Tryte const& tryte);
	int8_t static sign(Tryte const& tryte);

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
	arithmetic operators
	*/
	// add, ignoring carry
	Tryte operator+(Tryte const& other) const;
	Tryte& operator+=(Tryte const& other);
	// subtract, ignoring carry
	Tryte operator-(Tryte const& other) const;
	Tryte& operator-=(Tryte const& other);
	// flip sign
	Tryte operator-() const;

	/*
	stream extraction and insertion operators
	*/
	friend std::ostream& operator<<(std::ostream& os, Tryte const& tryte);
	friend std::istream& operator>>(std::istream& is, Tryte& tryte);

	// addition that respects the carry flag
	static Tryte add_with_carry(Tryte const& t1, Tryte const& t2, int8_t& carry);
	static Tryte add_with_carry(Tryte const& t1, Tryte const& t2, int8_t&& carry);
	// shift tritiwise
	static Tryte shift(Tryte const& t1, Tryte const& t2);

};
