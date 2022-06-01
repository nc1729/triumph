#include <vector>
#include <string>

#include "Token.h"
#include "Statement.h"
#include "constants.h"
#include "Block.h"
#include "error.h"
#include "parse.h"

std::vector<Block> parse::make_blocks(std::vector<Token> const& tokens)
{
	bool in_block = false;
	bool in_statement = false;

	
	std::vector<Block> blocks;
	std::string block_name;
	std::vector<Statement> block_statements;
	std::vector<Token> statement_tokens;

	size_t index = 0;
	while (index < tokens.size())
	{
		Token const& this_token = tokens[index];
		size_t line_number = this_token.line_number;
		if (!in_block)
		{
			if (this_token.type == TokenType::NAME)
			{
				// found a block name, keep skipping newlines until we find its start {
				while (index + 1 < tokens.size())
				{
					Token const& next_token = tokens[index + 1];
					if (next_token.type == TokenType::BLOCK_START)
					{
						// found a new block
						in_block = true;
						statement_tokens.clear();
						block_statements.clear();
						// token names start with underscores, so cut that away
						block_name = this_token.value.substr(1);
						index++;
						break;
					}
					else if (next_token.type == TokenType::NEWLINE)
					{
						index++;
					}
					else
					{
						// there shouldn't be anything other than names outside of blocks
						throw TASError("Expected block start token {", line_number);
					}
				}
			}
			else if (this_token.type == TokenType::NEWLINE)
			{
				// do nothing but move forward to next token
			}
			else
			{
				// there shouldn't be anything other than names outside of blocks
				throw TASError("Expected block start token {", line_number);
			}
		}
		else
		{
			// in a block, but not in a statement (just after a new line or semicolon)
			if (!in_statement)
			{
				if (this_token.type == TokenType::BLOCK_END)
				{
					// found the end of a block
					in_block = false;
					blocks.emplace_back(block_name, block_statements);
				}
				else if (this_token.type == TokenType::JUMP_LABEL)
				{
					// isolate jump labels into their own statements, even if they're not line separated
					statement_tokens.clear();
					statement_tokens.push_back(this_token);
					block_statements.emplace_back(statement_tokens);
				}
				else if (this_token.type == TokenType::NEWLINE || this_token.type == TokenType::STATEMENT_END)
				{
					// skip this token, empty statement
				}
				else
				{
					// found the start of a statement
					in_statement = true;
					statement_tokens.clear();
					statement_tokens.push_back(this_token);
				}
			}
			else
			{
				// in a block and in a statement, need to check if the statement has ended
				if (this_token.type == TokenType::BLOCK_END)
				{
					// found the end of a statement, and a block
					in_statement = false;
					in_block = false;
					block_statements.emplace_back(statement_tokens);
					blocks.emplace_back(block_name, block_statements);
				}
				else if (this_token.type == TokenType::NEWLINE || this_token.type == TokenType::STATEMENT_END)
				{
					// found the end of a statement
					in_statement = false;
					block_statements.emplace_back(statement_tokens);
				}
				else
				{
					statement_tokens.push_back(this_token);
				}
			}
		}
		index++;
	}
	
	return blocks;
}

std::vector<Statement> parse::expand_macro(Statement const& macro)
{
	std::vector<Statement> new_statements;
	if (macro[0].value == "NOT")
	{
		if (macro.size() != 2 || macro[1].type != TokenType::REG)
		{
			throw TASError("NOT macro takes 1 register argument", macro.line_number);
		}
		// "NOT X" is equivalent to "FLIP X"
		new_statements.push_back(Statement({ Token("FLIP", macro.line_number, TokenType::INSTR), macro[1] }));
	}
	else if (macro[0].value == "RET")
	{
		if (macro.size() != 1)
		{
			throw TASError("RET macro takes no arguments", macro.line_number);
		}
		// RET is equivalent to PJP
		new_statements.push_back(Statement({ Token("PJP", macro.line_number, TokenType::INSTR) }));
	}
	else if (macro[0].value == "CALL")
	{
		// check number of arguments
		if (macro.size() == 1)
		{
			throw TASError("CALL macro requires at least one NAME argument", macro.line_number);
		}
		else if (macro.size() > 10)
		{
			throw TASError("CALL macro can only accept up to nine arguments "
				"(the function to call and eight REG or VAL arguments)",
				macro.line_number);
		}

		// check first argument is a name of a code block
		if (macro[1].type != TokenType::NAME)
		{
			throw TASError("CALL macro must be followed by the name of a code block", macro.line_number);
		}

		size_t number_of_func_args = macro.size() - 2;
		// now begin constructing new_statements
		// first need to push registers onto stack - PUSH B; PUSH C; etc
		// A is not pushed - by convention A is the return value and we can expect it to be overwritten
		for (size_t i = 0; i < number_of_func_args; i++)
		{
			new_statements.push_back(
				Statement({
				Token("PUSH", macro.line_number, TokenType::INSTR),
				Token(constants::regs[i + 1], macro.line_number, TokenType::REG) }));
		}
		// now set registers B, C etc to be the values (or regs) given by the reg/value arguments
		for (size_t i = 0; i < number_of_func_args; i++)
		{
			new_statements.push_back(
				Statement({
				Token("SET", macro.line_number, TokenType::INSTR),
				Token(constants::regs[i + 1], macro.line_number, TokenType::REG),
				macro[i + 2] }));
		}
		// next, jump and store to the function argument
		new_statements.push_back(
			Statement({
				Token("JPS", macro.line_number, TokenType::JUMP_INSTR),
				macro[1] }));
		// function should contain a RET statement which will get us back here
		// (as long as the stack is cleared properly!)
		// now pop registers off the stack to restore state to before the CALL macro (except for register A)
		// need to pop in reverse order to the order we pushed them (if we pushed any...!)
		for (size_t i = 0; i < number_of_func_args; i++)
		{
			new_statements.push_back(
				Statement({
					Token("POP", macro.line_number, TokenType::INSTR),
					Token(constants::regs[number_of_func_args - i], macro.line_number, TokenType::REG) }));
		}
		// after all this, something like CALL func B, 72, 'a' should become
		// PUSH B; PUSH C; PUSH D; SET B, B (nop?); SET C, 72; SET D, 'a'; JPS func; POP D; POP C; POP B
		// a CALL function with n arguments will become 3n + 1 instructions, so it's a bit heavy
	}
	return new_statements;
}

std::vector<Block>& parse::handle_macros(std::vector<Block>& blocks)
{
	// loop through blocks, finding and fixing macros
	for (Block& block : blocks)
	{
		std::vector<Statement> new_statements;
		for (Statement const& statement : block)
		{
			if (statement.type == StatementType::MACRO)
			{
				std::vector<Statement> expanded_macro = expand_macro(statement);
				// add new statements from expanded macro onto new_statements
				new_statements.insert(new_statements.end(), expanded_macro.begin(), expanded_macro.end());
			}
			else
			{
				// nothing to expand, add it to new_statements
				new_statements.push_back(statement);
			}
		}
		block.statements = new_statements;
	}

	return blocks;
}

std::vector<Block> parse::parse(std::vector<Token> const& tokens)
{
	std::vector<Block> blocks = make_blocks(tokens);
	blocks = handle_macros(blocks);
	return blocks;
}