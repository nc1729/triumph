#include <vector>
#include <string>

#include "common/Tryte.h"
#include "Block.h"
#include "Statement.h"
#include "error.h"
#include "handle_instr.h"
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
    }
    else if (statement.type == StatementType::JUMP_LABEL)
    {
        // do nothing - handle these later
    }
    else {
        // statement is a JUMP or a INSTR
        // assemble this statement
        std::string instr = statement[0].value;
        auto handle = handle_instr.at(instr);
        handle(statement);
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
    // finish all strings with '\0' char - "000"
    string_trytes.push_back("000");
    return string_trytes;
}

void assemble::find_jump_label_offsets(Block& block)
{
    size_t offset = 0;
    for (Statement const& statement : block)
    {
        if (statement.type == StatementType::JUMP_LABEL)
        {
            if (block.jump_label_offsets.find(statement[0].value) != block.jump_label_offsets.end())
            {
                // redeclaration of jump label, throw an error
                std::string err_msg = "Redeclaration of jump label " + statement[0].value;
                throw TASError(err_msg, statement.line_number);
            }
            else
            {
                block.jump_label_offsets[statement[0].value] = offset;
            }
        }
        else
        {
            offset += statement.tryte_length();
        }
    }
}

std::vector<Block>& assemble::assemble(std::vector<Block>& blocks)
{
    for (Block& block : blocks)
    {
        // assemble statements in block
        for (Statement& statement : block)
        {
            assemble(statement);
        }

        // populate jump offset table
        find_jump_label_offsets(block);
        // compute length of blocks
        block.compute_tryte_length();
    }
    return blocks;
}