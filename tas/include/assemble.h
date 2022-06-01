#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>

#include "Block.h"
#include "Statement.h"

namespace assemble
{
    // check syntax of statement
    // if statement is syntatically incorrect (wrong number of args)
    // throw an error here
    void check_syntax(Statement const& statement);
    // assemble a statement - fill out its length and assembly members
    Statement& assemble(Statement& statement);
    // helper function to loop through chars in string and convert them to a list of Trytes
    std::vector<std::string> assemble_string(Token const& string_token);
    // once statements in a block have been assembled, find offsets of jump labels
    void find_jump_label_offsets(Block& block);
    // loop through statements in blocks, working out their equivalent in ternary
    std::vector<Block>& assemble(std::vector<Block>& blocks);
}