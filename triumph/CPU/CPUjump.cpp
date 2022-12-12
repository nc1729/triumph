#include "common/Tryte.h"
#include "CPU/CPU.h"

void CPU::decode_and_execute_jump(int8_t const mid, int8_t const low)
{
    // if more than two registers are used in a jump instruction,
    // the opcode will be two Trytes long - the second is for the registers
    // for example, JP [A], [B], [C] => jmj abc
    Tryte const& reg_tryte = memory_[pc_ + 1];
    int8_t const reg1 = reg_tryte.get_high();
	int8_t const reg2 = reg_tryte.get_mid();
    int8_t const reg3 = reg_tryte.get_low();

    // decode which jump statement this is and execute it
	switch (mid)
	{
	case -13:
		// jM* - JP $X, $Y, $Z
		// M = ---
		switch (low)
		{
        case 0:
            // jM0 - JP $X $Y $Z
            jump(memory_[pc_ + 1], memory_[pc_ + 2], memory_[pc_ + 3]);
		    instr_size_ = 4;
		    break;
		case 10:
			// jMj - JP $X
            // jump unconditionally to $X
			// special case (shorthand for JP $X, $X, $X)
			jump(memory_[pc_ + 1], memory_[pc_ + 1], memory_[pc_ + 1]);
			instr_size_ = 2;
            break;
        default:
            crash("Unrecognised instruction");
            break;
		}
        break;
	case -12:
		// jL* - JP $X, $Y, *
		// L = --0
        switch (low)
        {
        case 0:
            // jL0 - JP $X, $Y, *
            // example: JP $X, *, $Y => jL0 $X $Y
            jump(memory_[pc_ + 1], memory_[pc_ + 2], pc_ + 3);
            instr_size_ = 3;
            break;
        case 10:
            // jLj $X - JPNP $X
            // shorthand for JP $X, $X, *
            // if compare flag is not positive, jump to $X
            jump(memory_[pc_ + 1], memory_[pc_ + 1], pc_ + 2);
            instr_size_ = 2;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
		break;
	case -11:
		// jKZ - JP $X, $Y, [Z]
		// K = --+
		jump(memory_[pc_ + 1], memory_[pc_ + 2], regs_[low]);
		instr_size_ = 3;
		break;
	case -10:
        // jJ* - JP $X, *, $Y
        // J = -0-
        switch (low)
        {
        case 0:
            // jJ0 - JP $X, *, $Y
            // example: JP $X, *, $Y => jJ0 $X $Y
            jump(memory_[pc_ + 1], pc_ + 3, memory_[pc_ + 2]);
            instr_size_ = 3;
            break;
        case 10:
            // jJj $X - JPNZ $X
            // shorthand for JP $X, *, $X
            // if compare flag is not zero, jump to $X
            jump(memory_[pc_ + 1], pc_ + 2, memory_[pc_ + 1]);
            instr_size_ = 2;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
        break;
	case -9:
		// jI* - JP $X, *, *
		// I = -00
		// JPN $X
		jump(memory_[pc_ + 1], pc_ + 2, pc_ + 2);
		instr_size_ = 2;
		break;
	case -8:
		// jHY - JP $X, *, [Y]
		// H = -0+
		jump(memory_[pc_ + 1], pc_ + 2, regs_[low]);
		instr_size_ = 2;
		break;
	case -7:
		// jGY - JP $X, [Y], $Z
		// G = -+-
		jump(memory_[pc_ + 1], regs_[low], memory_[pc_ + 2]);
		instr_size_ = 3;
        break;
	case -6:
		// jFY - JP $X, [Y], *
		// F = -+0
		jump(memory_[pc_ + 1], regs_[low], pc_ + 2);
		instr_size_ = 2;
		break;
	case -5:
		// jE* - JP $X, [Y], [Z]
		// E = -++
        switch (low)
        {
            case 0:
                // jE0 - JP $X, [Y], [Z]
                // example: JP $X, [A], [B] => jE0 0ab $X
                jump(memory_[pc_ + 2], regs_[reg1], regs_[reg2]);
                instr_size_ = 3;
                break;
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                // jEY - JP $X, [Y], [Y] assembles to jEY $X
                jump(memory_[pc_ + 1], regs_[low], regs_[low]);
                instr_size_ = 2;
                break;
        }
        break;
    case -4:
        // jD* - JP *, $X, $Y
        // D = 0--
        switch (low)
        {
        case 0:
            // jD0 - JP *, $X, $Y
            jump(pc_ + 3, memory_[pc_ + 1], memory_[pc_ + 2]);
            instr_size_ = 3;
            break;
        case 10:
            // jDj $X - JPNN $X
            // shorthand for JP *, $X, $X
            // unconditional jump to [X]
            jump(pc_ + 2, memory_[pc_ + 1], memory_[pc_ + 1]);
            instr_size_ = 2;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
        break;
    case -3:
        // jC* - JP *, $X, *
        // C = 0-0
        // JPZ $X
        jump(pc_ + 2, memory_[pc_ + 1], pc_ + 2);
        instr_size_ = 2;
        break;
    case -2:
        // jBY - JP *, $X, [Y]
        // B = 0-+
        jump(pc_ + 2, memory_[pc_ + 1], regs_[low]);
        instr_size_ = 2;
        break;
    case -1:
        // jA* - JP *, *, $X
        // A = 00-
        // JPP $X
        jump(pc_ + 2, pc_ + 2, memory_[pc_ + 1]);
        instr_size_ = 2;
        break;
    case 0:
        // use this spare space for miscellaneous ops
        // JP *, *, * would just be a NOP anyway...
        switch (low)
        {
        case -10:
            // j0J - JPS $X
            jump_and_store(memory_[pc_ + 1]);
            instr_size_ = 2;
            break;
        case 0:
            // j00 - NOP
            nop();
            instr_size_ = 1;
            break;
        case 10:
            // j0j - PJP
            pop_and_jump();
            instr_size_ = 1;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            // j0X - JPS [X]
            jump_and_store(regs_[low]);
            instr_size_ = 1;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
        break;
    case 1:
        // jaX - JP *, *, [X]
        // a = 00+
        // JPP [X]
        jump(pc_ + 1, pc_ + 1, regs_[low]);
        instr_size_ = 1;
        break;
    case 2:
        // jbX - JP *, [X], $Y
        // b = 0+-
        jump(pc_ + 2, regs_[low], memory_[pc_ + 1]);
        instr_size_ = 2;
        break;
    case 3:
        // jcX - JP *, [X], *
        // c = 0+0
        // JPZ [X]
        jump(pc_ + 1, regs_[low], pc_ + 1);
        instr_size_ = 1;
        break;
    case 4:
        // jd* - JP *, [X], [Y]
        // d = 0++
        switch (low)
        {
        case 0:
            // jd0 - JP *, [X], [Y]
            // example: JP *, [A], [B] => jd0 0ab
            jump(pc_ + 2, reg_tryte[reg1], reg_tryte[reg2]);
            instr_size_ = 2;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            // jdX - JPNN [X]
            // shorthand for JP *, [X], [X]
            // unconditional jump to [X]
            jump(pc_ + 2, regs_[low], regs_[low]);
            instr_size_ = 1;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
        break;
    case 5:
        // jeX - JP [X], $Y, $Z
        // e = +--
        jump(regs_[low], memory_[pc_ + 1], memory_[pc_ + 2]);
        instr_size_ = 3;
        break;
    case 6:
        // jfX - JP [X], $Y, *
        // f = +-0
        jump(regs_[low], memory_[pc_ + 1], pc_ + 2);
        instr_size_ = 2;
        break;
    case 7:
        // jg* - JP [X], $Y, [Z]
        // g = +-+
        switch (low)
        {
            case 0:
                // jg0 - JP [X], $Y, [Z]
                // example: JP [A], $Y, B => jg0 a0b $Y
                jump(reg_tryte[reg1], memory_[pc_ + 2], reg_tryte[reg2]);
                instr_size_ = 3;
                break;
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                // jgX - JP [X], $Y, [X]
                // example: JP [C], $Y, [C] => jgc $Y
                jump(regs_[low], memory_[pc_ + 1], regs_[low]);
                instr_size_ = 2;
                break;
        }
        break;
    case 8:
        // jhX - JP [X], *, $Y
        // h = +0-
        jump(regs_[low], pc_ + 2, memory_[pc_ + 1]);
        instr_size_ = 2;
        break;
    case 9:
        // jiX - JP [X], *, *
        // i = +00
        // JPN [X]
        jump(regs_[low], pc_ + 1, pc_ + 1);
        instr_size_ = 1;
        break;
    case 10:
        // jj* - JP [X], *, [Y]
        // j = +0+
        switch (low)
        {
        case 0:
            // jj0 - JP [X], *, [Y]
            // example: JP [A], *, B => jj0 a0b
            jump(reg_tryte[reg1], pc_ + 2, reg_tryte[reg2]);
            instr_size_ = 2;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            // jjX - JPNZ [X]
            // shorthand for JP [X], *, [X]
            // unconditional jump to [X]
            jump(regs_[low], pc_ + 1, regs_[low]);
            instr_size_ = 1;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
	break;
    case 11:
        // jk* - JP [X], [Y], $Z
        // k = ++-
        switch (low)
        {
            case 0:
                // jk0 - JP [X], [Y], $Z
                // example: JP [A], [B], $Z => jk0 ab0 $Z
                jump(reg_tryte[reg1], reg_tryte[reg2], memory_[pc_ + 2]);
                instr_size_ = 3;
                break;
            case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                // jkX - JP [X], [X], $Y
                // example: JP [D], [D], $Y => jkd $Y
                jump(regs_[low], regs_[low], memory_[pc_ + 1]);
                instr_size_ = 2;
                break;
        }
        break;
    case 12:
        // jl* - JP [X], [Y], *
        // l = ++0
        switch (low)
        {
        case 0:
            // jl0 - JP [X], [Y], *
            // example: JP [X], [Y], * => jl0 XY0
            jump(reg_tryte[reg1], reg_tryte[reg2], pc_ + 2);
            instr_size_ = 2;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            // jlX - JPNP [X]
            // shorthand for JP [X], [X], *
            // if compare flag is not positive, jump to [X]
            jump(reg_tryte[low], reg_tryte[low], pc_ + 1);
            instr_size_ = 1;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
        break;
    case 13:
        // jm* - JP [X], [Y], [Z]
        // m = +++
        switch (low)
        {
        case 0:
            // jm0 - JP [X], [Y], [Z]
            jump(reg_tryte[reg1], reg_tryte[reg2], reg_tryte[reg3]);
            instr_size_ = 2;
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
            // jmX - JP [X]
            // shorthand for JP [X], [X], [X]
            // unconditional jump to [X]
            jump(regs_[low], regs_[low], regs_[low]);
            instr_size_ = 1;
            break;
        default:
            crash("Unrecognised instruction");
            break;
        }
	break;
    default:
        crash("Unrecognised instruction");
        break;
	}
}
