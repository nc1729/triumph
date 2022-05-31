#include <vector>
#include <string>

#include "Tryte.h"
#include "Block.h"
#include "Statement.h"
#include "error.h"
#include "assemble.h"

Statement& assemble::assemble(Statement& statement)
{
    std::vector<std::string> assembled_trytes;
    if (statement.type == StatementType::DATA)
    {
        for (Token const& token : statement)
        {
            if (token.type == TokenType::VAL)
            {
                assembled_trytes.push_back(token.value);
            }
            else if (token.type == TokenType::ADDR)
            {
                assembled_trytes.push_back(token.value);
            }
            else if (token.type == TokenType::STRING)
            {
                std::vector<std::string> string_trytes = assemble_string(token);
                assembled_trytes.insert(assembled_trytes.end(), string_trytes.begin(), string_trytes.end());
            }
            else
            {
                throw TASError("Unexpected token found in DATA statement", statement.line_number);
            }
        }
        statement.assembled_trytes = assembled_trytes;
        statement.length = statement.assembled_trytes.size();
    }
    else if (statement.type == StatementType::JUMP)
    {
        // can't assemble these now, but can compute their length
        statement.length = jump_statement_length(statement);
    }
    return statement;
}

std::vector<std::string> assemble::assemble_string(Token const& token)
{
    std::vector<std::string> string_trytes;
    for (char c : token.value)
    {
        // get ASCII value of char c, and convert it into its Tryte equivalent
        string_trytes.push_back(Tryte::get_str(Tryte(static_cast<int64_t>(c))));
    }
    return string_trytes;
}

size_t assemble::jump_statement_length(Statement const& statement)
{
    std::string instr = statement[0].value;
    if (instr == "JP")
    {
        if (statement.size() == 2)
        {
            // either JP [X] or JP $X
            if (statement[1].type == TokenType::REG_ADDR)
            {
                // JP [X]
                return 1;
            }
            else
            {
                // JP $X
                return 2;
            }
        }
    }
}

std::vector<Block>& assemble::assemble(std::vector<Block>& blocks)
{
    for (Block& block : blocks)
    {
        for (Statement& statement : block)
        {
            assemble(statement);
        }
    }
    return blocks;
}