#pragma once
#include <cstdint>
#include <map>
#include <iostream>
#include <array>

class Tryte
{
private:
	int8_t _high;
	int8_t _mid;
	int8_t _low;
	static std::map<char, int8_t> const schar_to_val =
	{ {'M', -13}, {'L', -12}, {'K', -11}, {'J', -10}, {'I', -9}, {'H', -8}, {'G', -7}, {'F', -6}, {'E', -5},
	  {'D',  -4}, {'C',  -3}, {'B',  -2}, {'A',  -1}, {'0',  0}, {'a',  1}, {'b',  2}, {'c',  3}, {'d',  4},
	  {'e',   5}, {'f',   6}, {'g',   7}, {'h',   8}, {'i',  9}, {'j', 10}, {'k', 11}, {'l', 12}, {'m', 13} };
	static char[] const schars = "MLKJIHGFEDCBA0abcdefghijklm";

public:
	/*
	constructors
	*/

	// default constructor (default value is 0)
	Tryte() : _high{ 0 }, _mid{ 0 }, _low{ 0 } {};

	// integer constructor
	Tryte(int64_t n);

	// char array constructor
	Tryte(char[4] const& s);

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
	tritwise shift operators
	*/
	Tryte operator<<(Tryte const& tryte) const;
	Tryte& operator<<=(Tryte const& tryte);
	Tryte operator<<(int64_t const n) const;
	Tryte& operator<<=(int64_t const n) const;
	Tryte operator>>(Tryte const& tryte) const;
	Tryte& operator>>=(Tryte const& tryte);
	Tryte operator>>(int64_t const n) const;
	Tryte& operator>>=(int64_t const n) const;

	/*
	arithmetic operators
	*/
	// add, ignoring carry
	Tryte operator+(Tryte const& other) const;
	Tryte& operator+=(Tryte const& other);
	// subtract, ignoring carry
	Tryte operator-(Tryte const& other) const;
	Tryte& operator -=(Tryte const& other);
	// flip sign
	Tryte operator-() const;

	/*
	stream extraction and insertion operators
	*/
	friend std::ostream& operator<<(std::ostream& os, Tryte const& tryte);
	friend std::istream& operator>>(std::istream& is, Tryte const& tryte);

};

// handle arithmetic in separate functions
Tryte add_with_carry(Tryte const& t1, Tryte const& t2, int8_t& carry);
Tryte sub_with_borrow(Tryte const& t1, Tryte const& t2, int8_t& carry);