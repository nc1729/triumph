#include <vector>
#include <string>

#include "Block.h"
#include "Statement.h"
#include "error.h"
#include "assemble.h"

void assemble::check_syntax(Statement const& statement)
{
    // check arguments of statement match the instruction
    std::string instr = statement[0].value;
    size_t number_of_arguments = statement.size() - 1;
    if (statement.type == StatementType::JUMP)
    {
        if (instr == "JP")
        {
            if (number_of_arguments == 1 || number_of_arguments == 3)
            {
                // check if any of the arguments are of incorrect type
                for (size_t i = 1; i < statement.size(); i++)
                {
                    if (!(statement[i].is_addr_type() || statement[i].type == TokenType::JUMP_PLACEHOLDER))
                    {
                        std::string err_string = "Argument " + std::to_string(i) + "of ";
                        err_string += std::to_string(number_of_arguments);
                        err_string += " in JP statement is not of ADDR type or a jump placeholder token *";
                        throw TASError(err_string, statement.line_number);
                    }
                }
            }
            else
            {
                std::string err_string = "JP expected 1 or 3 argument(s) but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "PJP")
        {
            if (number_of_arguments != 0)
            {
                std::string err_string = instr + " expected 0 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "JPS")
        {
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
        }
        else if (instr == "JPN")
        {
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
        }
        else if (instr == "JPZ")
        {
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
        }
        else if (instr == "JPP")
        {
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
        }
        else if (instr == "JPNN")
        {
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
        }
        else if (instr == "JPNZ")
        {
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
        }
        else if (instr == "JPNP")
        {
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
        }
    }
    else if (statement.type == StatementType::INSTR)
    {
        if (instr == "LOAD")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::ADDR || statement[2].type == TokenType::REG_ADDR))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of ADDR type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SAVE")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::ADDR || statement[2].type == TokenType::REG_ADDR))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of ADDR type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "BANK")
        {
            if (number_of_arguments == 1)
            {
                if (!(statement[1].type == TokenType::REG || statement[1].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "GANK")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SET")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                else if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SWAP")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                else if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "PUSH")
        {
            if (number_of_arguments == 1)
            {
                if (!(statement[1].type == TokenType::REG || statement[1].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "POP")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "PEEK")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "ADD")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "ADC")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SUB")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SBB")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "SH")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "INC")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "DEC")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "FLIP")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "ZERO")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "CMP")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "CPZ")
        {
            if (number_of_arguments == 1)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 1 argument but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "AND")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "OR")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "STAR")
        {
            if (number_of_arguments == 2)
            {
                if (statement[1].type != TokenType::REG)
                {
                    std::string err_string = "Argument 1 in " + instr + " statement must be of REG type";
                    throw TASError(err_string, statement.line_number);
                }
                if (!(statement[2].type == TokenType::REG || statement[2].type == TokenType::VAL))
                {
                    std::string err_string = "Argument 2 in " + instr + " statement must be of REG or VAL type";
                    throw TASError(err_string, statement.line_number);
                }
            }
            else
            {
                std::string err_string = instr + " expected 2 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "NOP")
        {
            if (number_of_arguments != 0)
            {
                std::string err_string = instr + " expected 0 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
        else if (instr == "HALT")
        {
            if (number_of_arguments != 0)
            {
                std::string err_string = instr + " expected 0 arguments but found " + std::to_string(number_of_arguments);
                throw TASError(err_string, statement.line_number);
            }
        }
    }
}