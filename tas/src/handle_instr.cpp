#include <string>
#include <vector>
#include <array>

#include "Statement.h"
#include "constants.h"
#include "error.h"
#include "handle_instr.h"

void handle::pjp(Statement& statement)
{
    // PJP
    // check argument syntax
    std::string instr = statement[0].value;
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = instr + " expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // PJP - j0j
    statement.assembled_trytes.push_back("j0j");
}

void handle::jps(Statement& statement)
{
    // JPS addr
    // check argument syntax
    std::string instr = statement[0].value;
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (!statement[1].is_addr_type())
        {
            std::string err_string = "Argument 1 in " + instr + " statement must be of ADDR type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[1].type == TokenType::ADDR || statement[1].type == TokenType::NAME)
    {
        // JPS $X - j0J $X
        statement.assembled_trytes.push_back("j0J");
        statement.assembled_trytes.push_back(statement[1].value);
    }
    else if (statement[1].type == TokenType::REG_ADDR)
    {
        // JPS [X] - j0X
        std::string opcode = "j0";
        opcode += statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        throw TASError("JPS must be followed by a single argument of ADDR type", statement.line_number);
    }
}