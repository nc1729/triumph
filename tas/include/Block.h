#pragma once

#include <vector>
#include <string>
#include <map>

#include "Tryte.h"
#include "Statement.h"

class Block
{
public:
	std::string name;
	std::string filename;
	std::vector<Statement> statements;
	Tryte address;
	size_t page_number = 0;
	size_t length = 0;
	std::map<std::string, size_t> jump_label_offsets;

	Block(std::string const& name, std::string const& filename, std::vector<Statement> const& statements) :
		name{ name }, filename{ filename }, statements{ statements } {};
	// allow construction of Block from single statement
	Block(std::string const& name, std::string const& filename, Statement const& statement) :
		Block(name, filename, std::vector<Statement>{statement}) {};
	
	std::vector<Statement>::iterator begin();
	std::vector<Statement>::iterator end();
	std::vector<Statement>::const_iterator begin() const;
	std::vector<Statement>::const_iterator end() const;

	// once statements in this block have been assembled, find the length of the
	// assembled statements in Trytes and set this->length to this
	void compute_tryte_length();
	// with assembling and linking finished, print the assembly from this block as a string
	std::string get_assembly() const;
};

std::ostream& operator<<(std::ostream& os, Block const& block);