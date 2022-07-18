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
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = "PJP expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // PJP - j0j
    statement.assembled_trytes.push_back("j0j");
}

void handle::jps(Statement& statement)
{
    // JPS addr
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (!statement[1].is_addr_type())
        {
            std::string err_string = "Argument 1 in JPS statement must be of ADDR type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "JPS expected 1 argument but found " + std::to_string(number_of_arguments);
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
        std::string opcode = "j00";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        throw TASError("JPS must be followed by a single argument of ADDR type", statement.line_number);
    }
}

void handle::load(Statement& statement)
{
    // LOAD reg, addr
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in LOAD statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (!(statement[2].is_addr_type()))
        {
            std::string err_string = "Argument 2 in LOAD statement must be of ADDR type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "LOAD expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::ADDR || statement[2].type == TokenType::NAME)
    {
        // LOAD X, $Y - mdX $Y
        std::string opcode = "md0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
    else
    {
        // LOAD X, [Y] - dXY
        std::string opcode = "d00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
}

void handle::save(Statement& statement)
{
    // SAVE reg, addr
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SAVE statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (!(statement[2].is_addr_type()))
        {
            std::string err_string = "Argument 2 in SAVE statement must be of ADDR type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SAVE expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::ADDR || statement[2].type == TokenType::NAME)
    {
        // SAVE X, $Y - mDX $Y
        std::string opcode = "mD0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
    else
    {
        // SAVE X, [Y] - DXY
        std::string opcode = "D00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
}

void handle::bank(Statement& statement)
{
    // BANK reg/val
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG && statement[1].type != TokenType::VAL)
        {
            std::string err_string = "Argument 1 in BANK statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "BANK expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[1].type == TokenType::REG)
    {
        // BANK X - mjX
        std::string opcode = "mj0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // BANK t9 - Mj0 t9
        statement.assembled_trytes.push_back("Mj0");
        statement.assembled_trytes.push_back(statement[1].value);
    }
}

void handle::gank(Statement& statement)
{
    // GANK reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in BANK statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "GANK expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // GANK X - mJX
    std::string opcode = "mJ0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::set(Statement& statement)
{
    // SET reg, reg/addr/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SET statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL
            && statement[2].type != TokenType::ADDR && statement[2].type != TokenType::NAME)
        {
            std::string err_string = "Argument 2 in SET statement must be of REG, VAL, ADDR or NAME type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SET expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // SET X, Y - aXY
        std::string opcode = "a00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // SET X, t9 - maX t9
        std::string opcode = "ma0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::swap(Statement& statement)
{
    // SWAP reg, reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SET statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG)
        {
            std::string err_string = "Argument 2 in SET statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SET expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // SWAP X, Y - AXY
    std::string opcode = "A00";
    opcode[1] = statement[1].value[0];
    opcode[2] = statement[2].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::push(Statement& statement)
{
    // PUSH reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG && statement[1].type != TokenType::VAL)
        {
            std::string err_string = "Argument 1 in PUSH statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "PUSH expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[1].type == TokenType::REG)
    {
        // PUSH X - mfX
        std::string opcode = "mf0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // PUSH t9 - Mf0 t9
        statement.assembled_trytes.push_back("Mf0");
        statement.assembled_trytes.push_back(statement[1].value);
    }
}

void handle::pop(Statement& statement)
{
    // POP reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in POP statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "POP expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // POP X - mFX
    std::string opcode = "mF0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::peek(Statement& statement)
{
    // PEEK reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in PEEK statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "PEEK expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // PEEK X - mmX
    std::string opcode = "mm0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::add(Statement& statement)
{
    // ADD reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in ADD statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in ADD statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "ADD expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // ADD X, Y - eXY
        std::string opcode = "e00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // ADD X, t9 - meX t9
        std::string opcode = "me0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::adc(Statement& statement)
{
    // ADC reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in ADC statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in ADC statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "ADC expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // ADC X, Y - eXY
        std::string opcode = "g00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // ADC X, t9 - mgX t9
        std::string opcode = "mg0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::sub(Statement& statement)
{
    // SUB reg, reg/val
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SUB statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in SUB statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SUB expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // SUB X, Y - EXY
        std::string opcode = "E00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // SUB X, t9 - meX t9
        std::string opcode = "mE0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::sbb(Statement& statement)
{
    // SBB reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SBB statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in SBB statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SBB expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // SBB X, Y - GXY
        std::string opcode = "G00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // SBB X, t9 - meX t9
        std::string opcode = "mG0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::sh(Statement& statement)
{
    // SH reg, reg/t3
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in SH statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in SH statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "SH expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // SH X, Y - hXY
        std::string opcode = "h00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // SH X, t3 - HX(t3)
        std::string opcode = "H00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[2]; // third char in value - truncate value to t3 (-13 <= x <= 13)
        statement.assembled_trytes.push_back(opcode);
    }
}

void handle::inc(Statement& statement)
{
    // INC reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in INC statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "INC expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // INC X - mlX
    std::string opcode = "ml0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::dec(Statement& statement)
{
    // DEC reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in DEC statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "DEC expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // DEC X - mLX
    std::string opcode = "mL0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::flip(Statement& statement)
{
    // FLIP reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in FLIP statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "FLIP expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // FLIP X - mMX
    std::string opcode = "mM0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::zero(Statement& statement)
{
    // ZERO reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in ZERO statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "ZERO expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // ZERO X - m0X
    std::string opcode = "m00";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::cmp(Statement& statement)
{
    // CMP reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in CMP statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in CMP statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "CMP expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // CMP X, Y - cXY
        std::string opcode = "c00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // CMP X, t9 - mcX t9
        std::string opcode = "mc0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::cpz(Statement& statement)
{
    // CPZ reg
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 1)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in CPZ statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "CPZ expected 1 argument but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    // CPZ X - mCX
    std::string opcode = "mC0";
    opcode[2] = statement[1].value[0];
    statement.assembled_trytes.push_back(opcode);
}

void handle::and_instr(Statement& statement)
{
    // AND reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in AND statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in AND statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "AND expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // AND X, Y - bXY
        std::string opcode = "b00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // AND X, t9 - mcX t9
        std::string opcode = "mb0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::or_instr(Statement& statement)
{
    // OR reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in OR statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in OR statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "OR expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // OR X, Y - BXY
        std::string opcode = "B00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // OR X, t9 - mBX t9
        std::string opcode = "mB0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::star(Statement& statement)
{
    // STAR reg, reg/t9
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments == 2)
    {
        if (statement[1].type != TokenType::REG)
        {
            std::string err_string = "Argument 1 in STAR statement must be of REG type";
            throw TASError(err_string, statement.line_number);
        }
        if (statement[2].type != TokenType::REG && statement[2].type != TokenType::VAL)
        {
            std::string err_string = "Argument 2 in STAR statement must be of REG or VAL type";
            throw TASError(err_string, statement.line_number);
        }
    }
    else
    {
        std::string err_string = "STAR expected 2 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // assemble
    if (statement[2].type == TokenType::REG)
    {
        // STAR X, Y - kXY
        std::string opcode = "k00";
        opcode[1] = statement[1].value[0];
        opcode[2] = statement[2].value[0];
        statement.assembled_trytes.push_back(opcode);
    }
    else
    {
        // STAR X, t9 - mkX t9
        std::string opcode = "mk0";
        opcode[2] = statement[1].value[0];
        statement.assembled_trytes.push_back(opcode);
        statement.assembled_trytes.push_back(statement[2].value);
    }
}

void handle::nop(Statement& statement)
{
    // NOP
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = "NOP expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    // NOP - j00
    statement.assembled_trytes.push_back("j00");
}

void handle::halt(Statement& statement)
{
    // HALT
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = "HALT expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    statement.assembled_trytes.push_back("000");
}

void handle::sleep(Statement& statement)
{
    // SLEEP
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = "SLEEP expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    statement.assembled_trytes.push_back("M00");
}

void handle::break_instr(Statement& statement)
{
    // BREAK
    size_t number_of_arguments = statement.size() - 1;
    if (number_of_arguments != 0)
    {
        std::string err_string = "BREAK expected 0 arguments but found " + std::to_string(number_of_arguments);
        throw TASError(err_string, statement.line_number);
    }

    statement.assembled_trytes.push_back("MMM");
}