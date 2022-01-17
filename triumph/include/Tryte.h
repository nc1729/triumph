#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <array>

class Tryte
{
private:
	int8_t _high;
	int8_t _mid;
	int8_t _low;
	static std::map<char, int8_t> const schar_to_val;
	static std::string const schars;

public:
	/*
	constructors
	*/

	// default constructor (default value is 0)
	Tryte() : _high{ 0 }, _mid{ 0 }, _low{ 0 } {};

	// integer constructor
	Tryte(int64_t n);

	// string constructor
	Tryte(std::string const& s);

	// septavingt array constructor
	Tryte(std::array<int8_t, 3> const& arr);

	// ternary array constructor
	Tryte(std::array<int8_t, 9> const& arr);

	/*
	useful functions
	*/
	int64_t static constexpr get_int(Tryte const& tryte);
	std::string static constexpr get_str(Tryte const& tryte);
	int8_t static constexpr sign(Tryte const& tryte);
	std::array<int8_t, 9> static constexpr ternary_array(Tryte const& tryte);

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
	friend std::istream& operator>>(std::istream& is, Tryte const& tryte);

	/*
	handle arithmetic involving carry in static functions
	*/
	static Tryte add_with_carry(Tryte const& t1, Tryte const& t2, int8_t& carry);
	static Tryte add_with_carry(Tryte const& t, int64_t const n, int8_t& carry);
	static Tryte sub_with_borrow(Tryte const& t1, Tryte const& t2, int8_t& carry);
	static Tryte sub_with_borrow(Tryte const& t, int64_t const n, int8_t& carry);
	static Tryte shift(Tryte const& t1, Tryte const& t2, int8_t& carry);

};
