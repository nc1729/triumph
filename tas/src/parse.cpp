#include <vector>
#include <string>
#include <map>

#include "Token.h"
#include "Statement.h"
#include "constants.h"
#include "Block.h"
#include "error.h"
#include "parse.h"

std::vector<Token> parse::handle_aliases(std::vector<Token> const& tokens)
{
	bool in_block = false;
	std::map<std::string, Token> global_aliases;
	std::map<std::string, Token> block_aliases;
	std::vector<Token> new_tokens;
	
	// loop through tokens, searching for aliases
	size_t index = 0;
	while (index < tokens.size())
	{
		Token const& this_token = tokens[index];
		if (in_block)
		{
			if (this_token.type == TokenType::ALIAS)
			{
				// found a local alias declaration
				// must be of form "alias NAME TOKEN", if not, throw an error here
				if (index + 2 >= tokens.size())
				{
					// there are not two tokens following this alias token, throw an error
					throw TASError("alias declaration must be of the form \"alias [NAME] [TOKEN]\"", this_token.line_number);
				}

				if (tokens[index + 1].type != TokenType::NAME)
				{
					// alias must be followed by a token of NAME type
					throw TASError("alias declaration must be of the form \"alias [NAME] [TOKEN]\"", this_token.line_number);
				}

				if (tokens[index + 2].type != TokenType::VAL && tokens[index + 2].type != TokenType::ADDR &&
					tokens[index + 2].type != TokenType::STRING && tokens[index + 2].type != TokenType::NAME)
				{
					// token to replace the alias must be VAL, ADDR, STRING or a NAME
					throw TASError("Invalid alias declaration - must alias a VAL, ADDR, STRING or NAME token", this_token.line_number);
				}

				// now add this alias to the local aliases...
				block_aliases.insert({tokens[index + 1].value, tokens[index + 2]});
				index += 2;
			}
			else if (this_token.type == TokenType::NAME)
			{
				// found a name - check if it is in the global or local aliases,
				// and replace this token if it is
				// note - local aliases supersede global aliases of the same name
				if (block_aliases.find(this_token.value) != block_aliases.end())
				{
					// found this name as a previously encountered local alias, so replace it
					new_tokens.push_back(block_aliases[this_token.value]);
				}
				else if (global_aliases.find(this_token.value) != global_aliases.end())
				{
					// found this name as a previously encountered global alias, so replace it
					new_tokens.push_back(global_aliases[this_token.value]);
				}
				else
				{
					// this name is not aliased, so don't touch it
					new_tokens.push_back(this_token);
				}
			}
			else if (this_token.type == TokenType::BLOCK_END)
			{
				// found the end of this block
				in_block = false;
				// aliases inside this block are local to this block, so forget them now
				block_aliases.clear(); 
				new_tokens.push_back(this_token);
			}
			else
			{
				// just a normal token, add it to the new list
				new_tokens.push_back(this_token);
			}
		}
		else
		{
			if (this_token.type == TokenType::ALIAS)
			{
				// found a global alias declaration
				// must be of form "alias NAME TOKEN", if not, throw an error here
				if (index + 2 >= tokens.size())
				{
					// there are not two tokens following this alias token, throw an error
					throw TASError("alias declaration must be of the form \"alias [NAME] [TOKEN]\"", this_token.line_number);
				}

				if (tokens[index + 1].type != TokenType::NAME)
				{
					// alias must be followed by a token of NAME type
					throw TASError("alias declaration must be of the form \"alias [NAME] [TOKEN]\"", this_token.line_number);
				}

				if (tokens[index + 2].type != TokenType::VAL && tokens[index + 2].type != TokenType::ADDR &&
					tokens[index + 2].type != TokenType::STRING && tokens[index + 2].type != TokenType::NAME)
				{
					// token to replace the alias must be VAL, ADDR, STRING or a NAME
					throw TASError("Invalid alias declaration - must alias a VAL, ADDR, STRING or NAME token", this_token.line_number);
				}

				// now add this alias to the global aliases...
				global_aliases.insert({tokens[index + 1].value, tokens[index + 2]});
				index += 2;
			}
			else if (this_token.type == TokenType::BLOCK_START)
			{
				// found the start of a block
				in_block = true;
				new_tokens.push_back(this_token);
			}
			else
			{
				// normal token (outside of a block, so a NAME or NEWLINE or STATEMENT_END)
				new_tokens.push_back(this_token);
			}
		}
		index++;
	}
	return new_tokens;
}

std::vector<Block> parse::make_blocks(std::vector<Token> const& tokens, std::string const& filename)
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
					blocks.emplace_back(block_name, filename, block_statements);
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
				else if (this_token.type == TokenType::BLOCK_START)
				{
					// can't start a new block in the middle of one already
					throw TASError("Unexpected block start token {", line_number);
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
					// add an additional HALT statement if this block is the main block
					if (block_name == "main")
					{
						statement_tokens.clear();
						statement_tokens.push_back(Token("HALT", line_number));
						block_statements.emplace_back(statement_tokens);
					}
					blocks.emplace_back(block_name, filename, block_statements);
				}
				else if (this_token.type == TokenType::NEWLINE || this_token.type == TokenType::STATEMENT_END)
				{
					// found the end of a statement
					in_statement = false;
					block_statements.emplace_back(statement_tokens);
				}
				else if (this_token.type == TokenType::BLOCK_START)
				{
					// can't start a new block in the middle of one already
					throw TASError("Unexpected block start token {", line_number);
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

std::vector<Statement> parse::expand_macro(Statement const& macro, std::string const& block_name)
{
	std::vector<Statement> new_statements;
	if (macro[0].value == "NOT")
	{
		if (macro.size() != 2)
		{
			throw TASError("NOT macro takes 1 argument", macro.line_number);
		}
		// NOT reg => FLIP reg
		new_statements.push_back(Statement({ Token("FLIP", macro.line_number, TokenType::INSTR), macro[1] }));
	}
	else if (macro[0].value == "RET")
	{
		if (macro.size() != 1)
		{
			throw TASError("RET macro takes no arguments", macro.line_number);
		}

		// in main block, RET is replaced with HALT
		if (block_name == "main")
		{
			// RET => HALT
			new_statements.push_back(Statement({ Token("HALT", macro.line_number, TokenType::INSTR) }));
		}
		else
		{
			// in any other block, RETs are PJPs
			// RET => PJP
			new_statements.push_back(Statement({ Token("PJP", macro.line_number, TokenType::INSTR) }));
		}
	}
	else if (macro[0].value == "JPN")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPN macro takes 1 argument", macro.line_number);
		}
		// JPN addr => JP addr, *, *
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
					   macro[1],
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER) })
		);
	}
	else if (macro[0].value == "JPZ")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPZ macro takes 1 argument", macro.line_number);
		}
		// JPZ addr => JP *, addr, *
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
					   macro[1],
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER) })
		);
	}
	else if (macro[0].value == "JPP")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPP macro takes 1 argument", macro.line_number);
		}
		// JPP addr => JP *, *, addr
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
					   Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
					   macro[1]})
		);
	}
	else if (macro[0].value == "JPNN")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPNN macro takes 1 argument", macro.line_number);
		}
		// JPNN addr => JP *, addr, addr
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
						Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
						macro[1],
						macro[1] })
		);
	}
	else if (macro[0].value == "JPNZ")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPNZ macro takes 1 argument", macro.line_number);
		}
		// JPNZ addr => JP addr, *, addr
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
						macro[1],
						Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER),
						macro[1] })
		);
	}
	else if (macro[0].value == "JPNP")
	{
		if (macro.size() != 2)
		{
			throw TASError("JPNP macro takes 1 argument", macro.line_number);
		}
		// JPNP addr => JP addr, addr, *
		new_statements.push_back(
			Statement({ Token("JP", macro.line_number, TokenType::INSTR),
						macro[1],
						macro[1],
						Token("*", macro.line_number, TokenType::JUMP_PLACEHOLDER) })
		);
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
				Token(constants::regs[i + 1], macro.line_number) }));
		}
		// now set registers B, C etc to be the values (or regs) given by the reg/value arguments
		for (size_t i = 0; i < number_of_func_args; i++)
		{
			Token reg_token = Token(constants::regs[i + 1], macro.line_number);
			// check that we're not adding redundant ops like SET B, B that do nothing
			if (reg_token != macro[i + 2])
			{
				new_statements.push_back(
				Statement({
				Token("SET", macro.line_number, TokenType::INSTR),
				reg_token, macro[i + 2] }));
			}
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
					Token(constants::regs[number_of_func_args - i], macro.line_number) }));
		}
		// after all this, something like CALL func C, 72, 'a' should become
		// PUSH B; PUSH C; PUSH D; SET B, C; SET C, 72; SET D, 'a'; JPS func; POP D; POP C; POP B
		// a CALL function with n arguments will become 3n + 1 instructions, so it's a bit heavy
		// it's more efficient to use B for first argument, C for second, and so on
	}
	return new_statements;
}

std::vector<Block>& parse::add_last_halt(std::vector<Block>& blocks)
{
	for (Block& block : blocks)
	{
		if (block.name == "main")
		{
			std::vector<Token> new_tokens;
			if (block.statements.size() == 0)
			{
				// main block was empty, add a HALT instruction
				new_tokens.push_back(Token("HALT", 1, TokenType::INSTR));
				block.statements.emplace_back(new_tokens);
			}
			else
			{
				Statement& last_statement = block.statements.back();
				if (last_statement.type == StatementType::INSTR || last_statement.type == StatementType::JUMP)
				{
					if (last_statement[0].value != "HALT")
					{
						// main block doesn't end in a HALT statement, so add one here
						new_tokens.push_back(Token("HALT", last_statement.line_number, TokenType::INSTR));
						block.statements.emplace_back(new_tokens);
					}
				}
			}
		}
	}
	return blocks;
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
				std::vector<Statement> expanded_macro = expand_macro(statement, block.name);
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

std::vector<Block> parse::parse(std::vector<Token>& tokens, std::string const& filename)
{
	// search for alias statements and resolve them
	tokens = handle_aliases(tokens);
	// collect together code blocks, filling them with statements
	std::vector<Block> blocks = make_blocks(tokens, filename);
	// expand macro statements ("CALL", "JPZ", etc)
	blocks = handle_macros(blocks);
	// if there's a main block, ensure that it ends in a HALT, if it doesn't already
	blocks = add_last_halt(blocks);
	return blocks;
}