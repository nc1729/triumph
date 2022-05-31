#pragma once

#include <vector>

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
    // loop through statements in blocks, working out their equivalent in ternary
    std::vector<Block>& assemble(std::vector<Block>& blocks);
}