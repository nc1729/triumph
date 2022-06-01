#include <string>
#include <vector>
#include <array>

#include "Statement.h"
#include "constants.h"
#include "error.h"
#include "handle_instr.h"

void handle::jp(Statement& statement)
{
	// check JP statement has correct number of arguments
	std::string instr = statement[0].value;
	size_t number_of_args = statement.size() - 1;
    if (number_of_args == 1 || number_of_args == 3)
    {
        // check if any of the arguments are of incorrect type
        for (size_t i = 1; i < statement.size(); i++)
        {
            if (!(statement[i].is_addr_type() || statement[i].type == TokenType::JUMP_PLACEHOLDER))
            {
                std::string err_string = "Argument " + std::to_string(i) + "of ";
                err_string += std::to_string(number_of_args);
                err_string += " in JP statement is not of ADDR type or a jump placeholder token *";
                throw TASError(err_string, statement.line_number);
            }
        }
    }
    else
    {
        std::string err_string = "JP expected 1 or 3 argument(s) but found " + std::to_string(number_of_args);
        throw TASError(err_string, statement.line_number);
    }

    // now assemble (leaving placeholders behind)
    if (number_of_args == 1)
    {
        if (statement[1].type == TokenType::ADDR)
        {
            // JP $X
            statement.assembled_trytes.push_back("jMj");
            statement.assembled_trytes.push_back(statement[1].value);
            
        }
        else if (statement[1].type == TokenType::REG_ADDR)
        {
            // JP [X]
            std::string instr_tryte = "jm";
            instr_tryte += statement[1].value;
            statement.assembled_trytes.push_back(instr_tryte);
            
        }
        else if (statement[1].type == TokenType::NAME)
        {
            // JP name
            statement.assembled_trytes.push_back("jMj");
            statement.assembled_trytes.push_back(statement[1].value);
            
        }
        else if (statement[1].type == TokenType::JUMP_PLACEHOLDER)
        {
            // JP * - this is a NOP
            // could remove it, but presumably the programmer intended it to be here
            statement.assembled_trytes.push_back("j00");
            
        }
    }
    else if (number_of_args == 3)
    {
        // work out which type of arg is in which position
        std::array<int64_t, 3> arg_flags;
        for (size_t i = 0; i < 3; i++)
        {
            if (statement[i + 1].type == TokenType::ADDR || statement[i + 1].type == TokenType::NAME)
            {
                arg_flags[i] = -1;
            }
            else if (statement[i + 1].type == TokenType::JUMP_PLACEHOLDER)
            {
                arg_flags[i] = 0;
            }
            else if (statement[i + 1].type == TokenType::REG_ADDR)
            {
                arg_flags[i] = 1;
            }
            else
            {
                std::string err_string = "Argument " + std::to_string(i + 1) " of 3 in JP statement must be of ADDR type";
                throw TASError(err_string, statement.line_number);
            }
        }

        // with the arg_flags, calculate the opcode and its following trytes
        std::string opcode = "j00";
        char& code_char = opcode[1]; // default '0'
        char& third_char = opcode[2]; // default '0'
        // each of the 27 possible jump instructions corresponds to a three trit code, e.g. -+0
        // where the value of the nth trit: 
        // - means that arg is an addr/name
        // 0 means that arg is a jump placeholder
        // + means that arg is a reg
        code_char = constants::septavingt_chars[13 + (9 * arg_flags[0]) + (3 * arg_flags[1]) + arg_flags[2]];
        // opcode will be j + id_char + third_char, where third_char is determined in various ways

        // and work through all 27 (!) cases separately
        if (code_char == 'M')
        {
            // ---
            // JP $X, $Y, $Z
            if (statement[1] == statement[2] && statement[2] == statement[3])
            {
                // special case
                // JP $X
                // jMj $X
                third_char = 'j';
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
            }
            else
            {
                // general case
                // jM0 $X $Y $Z
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
                statement.assembled_trytes.push_back(statement[2].value);
                statement.assembled_trytes.push_back(statement[3].value);
            }
        }
        else if (code_char == 'L')
        {
            // --0
            // JP $X, $Y, *
            if (statement[1] == statement[2])
            {
                // special case
                // JP $X, $X, *
                // JPNP $X
                // jLj $X
                third_char = 'j';
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
            }
            else
            {
                // general case
                // jL0 $X $Y
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
                statement.assembled_trytes.push_back(statement[2].value);
            }
        }
        else if (code_char == 'K')
        {
            // --+
            // JP $X, $Y, [Z]
            // jKZ $X $Y
            third_char = statement[3].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[1].value);
            statement.assembled_trytes.push_back(statement[2].value);
        }
        else if (code_char == 'J')
        {
            // -0-
            // JP $X, *, $Y
            if (statement[1] == statement[3])
            {
                // special case
                // JP $X, *, $X
                // JPNZ $X
                // jJj $X
                third_char = 'j';
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
            }
            else
            {
                // general case
                // JP $X, *, $Y
                // jJ0 $X $Y
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
                statement.assembled_trytes.push_back(statement[3].value);
            }
        }
        else if (code_char == 'I')
        {
            // -00
            // JP $X, *, *
            // JPN $X
            // jI0 $X
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[1].value);
        }
        else if (code_char == 'H')
        {
            // -0+
            // JP $X, *, [Y]
            // jHY $X
            third_char = statement[3].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[1].value);
        }
        else if (code_char == 'G')
        {
            // -+-
            // JP $X, [Y], $Z
            // jGY $X $Z
            third_char = statement[2].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[1].value);
            statement.assembled_trytes.push_back(statement[3].value);
        }
        else if (code_char == 'F')
        {
            // -+0
            // JP $X, [Y], *
            // jFY $X
            third_char = statement[2].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[1].value);
        }
        else if (code_char == 'E')
        {
            // -++
            // JP $X, [Y], [Z]
            if (statement[2] == statement[3])
            {
                // special case
                // JP $X, [Y], [Y]
                // jEY $X
                third_char = statement[2].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[1].value);
            }
            else
            {
                // general case
                // JP $X, [Y], [Z]
                // jE0 0YZ $X
                std::string reg_tryte = "000";
                reg_tryte[1] = statement[2].value[0];
                reg_tryte[2] = statement[3].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
                statement.assembled_trytes.push_back(statement[1].value);
            }
        }
        else if (code_char == 'D')
        {
            // 0--
            // JP *, $X, $Y
            if (statement[2] == statement[3])
            {
                // special case
                // JP *, $X, $X
                // JPNN $X
                // jDj $X
                third_char = 'j';
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[2].value);
            }
            else
            {
                // general case
                // JP *, $X, $Y
                // jD0 $X $Y
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[2].value);
                statement.assembled_trytes.push_back(statement[3].value);
            }
        }
        else if (code_char == 'C')
        {
            // 0-0
            // JP *, $X, *
            // JPZ $X
            // jC0 $X
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[2].value);
        }
        else if (code_char == 'B')
        {
            // 0-+
            // JP *, $X, [Y]
            // jBY $X
            third_char = statement[2].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[2].value);
        }
        else if (code_char == 'A')
        {
            // 00-
            // JP *, *, $X
            // JPP $X
            // jA0 $X
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[3].value);
        }
        else if (code_char == '0')
        {
            // 000
            // JP *, *, *
            // j00 - a NOP - could ignore, but will leave alone
            statement.assembled_trytes.push_back(opcode);
        }
        else if (code_char == 'a')
        {
            // 00+
            // JP *, *, [X]
            // JPP [X]
            // jaX
            third_char = statement[3].value[0];
            statement.assembled_trytes.push_back(opcode);
        }
        else if (code_char == 'b')
        {
            // 0+-
            // JP *, [X], $Y
            // jbX $Y
            third_char = statement[2].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[3].value);
        }
        else if (code_char == 'c')
        {
            // 0+0
            // JP *, [X], *
            // JPZ [X]
            // jcX
            third_char = statement[2].value[0];
            statement.assembled_trytes.push_back(opcode);
        }
        else if (code_char == 'd')
        {
            // 0++
            // JP *, [X], [Y]
            if (statement[2] == statement[3])
            {
                // JP *, [X], [X]
                // JPNN [X]
                // jdX
                third_char = statement[2].value[0];
                statement.assembled_trytes.push_back(opcode);
            }
            else
            {
                // general case
                // JP *, [X], [Y]
                // jd0 0XY
                std::string reg_tryte = "000";
                reg_tryte[1] = statement[2].value[0];
                reg_tryte[2] = statement[3].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
            }
        }
        else if (code_char == 'e')
        {
            // +--
            // JP [X], $Y, $Z
            // jeX $Y $Z
            third_char = statement[1].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[2].value);
            statement.assembled_trytes.push_back(statement[3].value);
        }
        else if (code_char == 'f')
        {
            // +-0
            // JP [X], $Y, *
            // jfX $Y
            third_char = statement[1].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[2].value);
        }
        else if (code_char == 'g')
        {
            // +-+
            // JP [X], $Y, [Z]
            if (statement[1] == statement[3])
            {
                // special case
                // JP [X], $Y, [X]
                // jgX $Y
                third_char = statement[1].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[2].value);
            }
            else
            {
                // general case
                // JP [X], $Y, [Z]
                // jg0 X0Z $Y
                std::string reg_tryte = "000";
                reg_tryte[0] = statement[1].value[0];
                reg_tryte[2] = statement[3].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
                statement.assembled_trytes.push_back(statement[2].value);
            }
        }
        else if (code_char == 'h')
        {
            // +0-
            // JP [X], *, $Y
            // jhX $Y
            third_char = statement[1].value[0];
            statement.assembled_trytes.push_back(opcode);
            statement.assembled_trytes.push_back(statement[3].value);
        }
        else if (code_char == 'i')
        {
            // +00
            // JP [X], *, *
            // JPN [X]
            // jiX
            third_char = statement[1].value[0];
            statement.assembled_trytes.push_back(opcode);
        }
        else if (code_char == 'j')
        {
            // +0+
            // JP [X], *, [Y]
            if (statement[1] == statement[3])
            {
                // special case
                // JP [X], *, [X]
                // JPNZ [X]
                // jjX
                third_char = statement[1].value[0];
                statement.assembled_trytes.push_back(opcode);
            }
            else
            {
                // general case
                // JP [X], *, [Y]
                // jj0 X0Y
                std::string reg_tryte = "000";
                reg_tryte[0] = statement[1].value[0];
                reg_tryte[2] = statement[3].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
            }
        }
        else if (code_char == 'k')
        {
            // ++-
            // JP [X], [Y], $Z
            if (statement[1] == statement[2])
            {
                // special case
                // JP [X], [X], $Y
                // jkX $Y
                third_char = statement[1].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(statement[3].value);
            }
            else
            {
                // general case
                // JP [X], [Y], $Z
                // jk0 XY0 $Z
                std::string reg_tryte = "000";
                reg_tryte[0] = statement[1].value[0];
                reg_tryte[1] = statement[2].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
                statement.assembled_trytes.push_back(statement[3].value);
            }
        }
        else if (code_char == 'l')
        {
            // ++0
            // JP [X], [Y], *
            if (statement[1] == statement[2])
            {
                // special case
                // JP [X], [X], *
                // JPNP [X]
                // jlX
                third_char = statement[1].value[0];
                statement.assembled_trytes.push_back(opcode);
            }
            else
            {
                // general case
                // JP [X], [Y], *
                // jl0 XY0
                std::string reg_tryte = "000";
                reg_tryte[0] = statement[1].value[0];
                reg_tryte[1] = statement[2].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
            }
        }
        else if (code_char == 'm')
        {
            // +++
            // JP [X], [Y], [Z]
            if (statement[1] == statement[2] && statement[2] == statement[3])
            {
                // special case
                // JP [X], [X], [X]
                // JP [X]
                // jmX
                third_char = statement[1].value[0];
                statement.assembled_trytes.push_back(opcode);
            }
            else
            {
                // general case
                // JP [X], [Y], [Z]
                // jm0 XYZ
                std::string reg_tryte = "000";
                reg_tryte[0] = statement[1].value[0];
                reg_tryte[1] = statement[2].value[0];
                reg_tryte[2] = statement[3].value[0];
                statement.assembled_trytes.push_back(opcode);
                statement.assembled_trytes.push_back(reg_tryte);
            }
        }
    }
    return;
}