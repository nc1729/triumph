#include <vector>
#include <string>

#include "Block.h"
#include "Statement.h"
#include "error.h"
#include "assemble.h"

void assemble::check_syntax(Statement const& statement)
{
    if (statement.type == StatementType::JUMP)
    {
        std::string instr = statement[0].value;
        size_t number_of_arguments = statement.size() - 1;
        if (instr == "JP")
        {
            bool arg_check = true;
            if (number_of_arguments == 1 || number_of_arguments == 3)
            {
                for (size_t i = 1; i < statement.size(); i++)
                {
                    if (!(statement[i].is_addr_type() || statement[i].type == TokenType::JUMP_PLACEHOLDER))
                    {
                        arg_check = false;
                    }
                }
            }
            else
            {
                arg_check = false;
            }

            if (!arg_check)
            {
                std::string err_string = "JP instruction must be of the form JP [ADDR] or JP [ADDR1], [ADDR2], [ADDR3]\n";
                err_string += "where [ADDR] is of ADDR type or a jump placeholder token *";
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "JPS")
        {
            // fill out all instructions
        }
    }
}
Statement& assemble::assemble(Statement& statement)
{
    return statement;
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