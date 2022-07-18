#pragma once

#include <string>
#include <map>
#include <functional>

#include "Statement.h"

namespace handle
{
	// functions for assembling particular instructions
	// called through handle_instr
	void jp(Statement& statement);
	void pjp(Statement& statement);
	void jps(Statement& statement);
	void load(Statement& statement);
	void save(Statement& statement);
	void bank(Statement& statement);
	void gank(Statement& statement);
	void set(Statement& statement);
	void swap(Statement& statement);
	void push(Statement& statement);
	void pop(Statement& statement);
	void peek(Statement& statement);
	void add(Statement& statement);
	void adc(Statement& statement);
	void sub(Statement& statement);
	void sbb(Statement& statement);
	void sh(Statement& statement);
	void inc(Statement& statement);
	void dec(Statement& statement);
	void flip(Statement& statement);
	void zero(Statement& statement);
	void cmp(Statement& statement);
	void cpz(Statement& statement);
	// and and or are C++ keywords...
	void and_instr(Statement& statement);
	void or_instr(Statement& statement);
	void star(Statement& statement);
	void nop(Statement& statement);
	void halt(Statement& statement);
	void sleep(Statement& statement);
	void break_instr(Statement& statement);
}

std::map<std::string, std::function<void(Statement&)>> const handle_instr =
{
	{"JP", &handle::jp},
	{"PJP", &handle::pjp},
	{"JPS", &handle::jps},
	{"LOAD", &handle::load},
	{"SAVE", &handle::save},
	{"BANK", &handle::bank},
	{"GANK", &handle::gank},
	{"SET", &handle::set},
	{"SWAP", &handle::swap},
	{"PUSH", &handle::push},
	{"POP", &handle::pop},
	{"PEEK", &handle::peek},
	{"ADD", &handle::add},
	{"ADC", &handle::adc},
	{"SUB", &handle::sub},
	{"SBB", &handle::sbb},
	{"SH", &handle::sh},
	{"INC", &handle::inc},
	{"DEC", &handle::dec},
	{"FLIP", &handle::flip},
	{"ZERO", &handle::zero},
	{"CMP", &handle::cmp},
	{"CPZ", &handle::cpz},
	{"AND", &handle::and_instr},
	{"OR", &handle::or_instr},
	{"STAR", &handle::star},
	{"NOP", &handle::nop},
	{"HALT", &handle::halt},
	{"SLEEP", &handle::sleep},
	{"BREAK", &handle::break_instr}
};