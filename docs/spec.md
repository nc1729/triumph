# TRIUMPH ternary computer

## Description

TRIUMPH is a ternary computer, a computer that runs on ternary logic and where the basic unit of computation is the *tryte*, rather than the byte. Rather than using binary arithmetic, this computer uses balanced ternary (base 3) arithmetic with the characters '-' for -1, '0' for 0, and '+' for +1. A description of balanced ternary can be found [here](https://en.wikipedia.org/wiki/Balanced_ternary).

Just as binary computers use the hexadecimal number system as a convenient shorthand for binary, this balanced ternary computer used a balanced *septavingtesmal* (base 27) number system, where the allowed characters M-A, 0, a-m correspond to the following balanced ternary values:

M : --- : -13
L : --0 : -12
K : --+ : -11
J : -0- : -10
I : -00 : -9
H : -0+ : -8
G : -+- : -7
F : -+0 : -6
E : -++ : -5
D : 0-- : -4
C : 0-0 : -3
B : 0-+ : -2
A : 00- : -1
0 : 000 : 0
a : 00+ : 1
b : 0+- : 2
c : 0+0 : 3
d : 0++ : 4
e : +-- : 5
f : +-0 : 6
g : +-+ : 7
h : +0- : 8
i : +00 : 9
j : +0+ : 10
k : ++- : 11
l : ++0 : 12
m : +++ : 13

For example, the decimal number 1000 can be written 729 + 27 * 10 + 1 = aja, and the value MMM = (-13 * 729) - (13 * 27) - 13 = -9,841. Flipping the sign of a balanced septavingtesmal number is equivalent to changing the number from upper to lower case.

A *tryte* is a balanced septavingtesmal number from -9,841 to 9,841 (MMM to mmm). 
## Specifications

- little-endian ternary computer
- 9 9-trit general purpose registers
- 9-trit data width and address space (capable of addressing 19,683 trytes)
- Memory bank system allowing for up to 19,683 memory banks of storage, each with 6,561 Trytes. These memory banks facilitate device I/O by direct memory access, rather than dedicated I/O instructions. For example, internal VRAM is accessible through this memory bank interface.

## Registers

9 9-trit registers:

A - general purpose (GP) register
B - GP register
C - GP register
D - GP register
E - GP register
F - flags, from least signficant trit to most significant trit:
    COMPARE - stores result of CMP instruction
    CARRY - ADD/ADC instructions can produce carry
    SIGN - sign of result of last arithmetic/logic operation
    STACK - stack status
G - GP register
H - GP register
I - GP register

in memory, also have the PC (program counter), the SP (stack pointer and the current MB (memory bank).

## Memory map

TRIUMPH can address Tryte memory in the range $MMM-$mmm.
By default, stack pointer SP begins at $m00.

$MMM-$Emm : 6561 trytes of general purpose memory (bank switchable)
$DMM-$dmm : 6561 trytes of general purpose memory
$eMM-$mMM : PROGRAM memory; assembled code is written here. Use at own risk.
$mMM-$m0A : stack; stack pointer SP begins at $m00 and decrements as stack grows
$m00-$mmj : reserved for system use (boot code, etc)
$mmk      : current memory bank MB
$mml      : stack pointer SP
$mmm      : program counter PC

## Instruction set

The following is a list of ternary assembly instructions accepted by TRIUMPH. Each instruction has a unique 1-tryte opcode (except for certain permissible JP instructions), with these opcodes being denoted "*XY" (for a standard 2-register instruction), "**X" (for a 1-register instruction) or "***", where '*' is a valid septavingtesmal character (M-A,0,a-m), and X/Y are 3-trit opcode representations of specific registers, given by the following table:

A - 00+ ('a')
B - 0+- ('b')
C - 0+0 ('c')
D - 0++ ('d')
E - +-- ('e')
F - +-0 ('f')
G - +-+ ('g')
H - +0- ('h')
I - +00 ('i')

For example, the general form of the opcode for "ADD X, Y" is eXY - "ADD A, B" is given by "eab". If instructions require raw memory addresses, these will immediately follow the opcode. For example, the jump instruction "JP $X" assembles to "jMj X", where X is the raw tryte address of $X. If an opcode contains the wildcard character '*', any valid septavingtesmal character is valid there (usually this is chosen to be 0.)

The instructions also permit indirect addressing of memory using registers, denoted by square brackets \[\] - for example, if register A = a00, \[A\] will point to the contents of memory at address $a00. The placeholder 't9' represents a 9-trit immediate value, standing for any value between -9,841 and 9,841. 't3' represents a 3-trit immediate value, standing for any value between -13 and 13. Values outside these ranges will be truncated.

### Memory management instructions

#### LOAD X, $Y
- Opcode: mdX $Y
- Length: 2 trytes
- Description: Load register Y with contents of address $X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### LOAD X, \[Y\]
- Opcode: dXY
- Length: 1 tryte
- Description: Load register Y with contents of indirect address given by register X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### SAVE X, $Y
- Opcode: mDX $Y
- Length: 2 trytes
- Description: Save contents of register X to address $Y.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### SAVE X, \[Y\]
- Opcode: DXY
- Length: 1 tryte
- Description: Save contents of register X to indirect address given by register Y.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### BANK X
- Opcode: mjX
- Length: 1 tryte
- Description: Set current BANK to value of register X.

#### BANK t9
- Opcode: Mj* t9
- Length: 2 trytes
- Description: Set current BANK to value of immediate t9.

#### GANK X
- Opcode: mJX
- Length: 1 tryte
- Description: Set register X equal to the current BANK value.

### Register management

#### SET X, Y
- Opcode: aXY
- Length: 1 tryte
- Description: set register X equal to register Y.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### SET X, t9
- Opcode: maX t9
- Length: 2 trytes
- Description: set register X equal to immediate value t9. t9 can be a value type, an addr type (in which case the address itself is used as the immediate, rather than its contents - use LOAD instruction to access memory) or a placeholder name type (useful for setting registers to addresses of code blocks)
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### SWAP X, Y
- Opcode: AXY
- Length: 1 tryte
- Description: swap the values of registers X and Y.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### Stack management

#### PUSH X
- Opcode: mfX
- Length: 1 tryte
- Description: decrement the stack pointer SP and write contents of X to that point in memory.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after decrementing, SP < mMM, stack flag will be set to '+', indicating overflow.

#### PUSH t9
- Opcode: Mf* t9
- Length: 2 trytes
- Description: decrement the stack pointer SP and write immediate value t9 to that point in memory.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after decrementing, SP < mMM, stack flag will be set to '+', indicating overflow.

#### POP X
- Opcode: mFX
- Length: 1 tryte
- Description: write value at $SP to register X, and increment the stack pointer.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after incrementing, SP > m00, stack flag will be set to '-', indicating underflow.

#### PEEK X
- Opcode: mmX
- Length: 1 tryte
- Description: write value at $SP to register X. Stack pointer is unchanged.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### Arithmetic operations

#### ADD X, Y
- Opcode: eXY
- Length: 1 tryte
- Description: X += Y
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: set with sign of result.
- Stack flag: No effect.

#### ADD X, t9
- Opcode: meX t9
- Length: 2 trytes
- Description: X += t9
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### ADC X, Y
- Opcode: gXY
- Length: 1 tryte
- Description: X += (Y + c), where 'c' is the current value in the carry flag.
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### ADC X, t9
- Opcode: mgX t9
- Length: 2 trytes
- Description: X += (t9 + c), where 'c' is the current value in the carry flag.
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SUB X, Y
- Opcode: EXY
- Length: 1 tryte
- Description: X -= Y
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SUB X, t9
- Opcode: mEX t9
- Length: 2 trytes
- Description: X -= t9
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SBB X, Y
- Opcode: GXY
- Length: 1 tryte
- Description: X -= (Y - b), where 'b', the "borrow" is the current value in the carry flag.
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SBB X, t9
- Opcode: mGX t9
- Length: 1 tryte
- Description: X -= (t9 - b), where 'b', the "borrow" is the current value in the carry flag.
- Compare flag: No effect.
- Carry flag: Carry flag will be set; '+' if overflow is positive, '0' if no overflow, '-' if overflow is negative.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SH X, Y
- Opcode: hXY
- Length: 1 tryte
- Description: Trit-shift the value of register X left or right by the value of the three least-significant trits of register Y. A positive number means shift left, i.e. multiply by 3 repeatedly. Negative means shift right. If Y == 0, nothing happens.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SH X, t3
- Opcode: HX(t3)
- Length: 1 tryte
- Description: Trit-shift the value of register X left or right by the value of t3. A positive number means shift left, i.e. multiply by 3 repeatedly. Negative means shift right. SH X, 0 does nothing.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### INC X
- Opcode: mlX
- Length: 1 tryte
- Description: X += 1.
- Compare flag: No effect.
- Carry flag: If increment overflows, carry flag will be set to '+'.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### DEC X
- Opcode: mLX
- Length: 1 tryte
- Description: X -= 1.
- Compare flag: No effect.
- Carry flag: If increment underflows, carry flag will be set to '-'.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### FLIP X
- Opcode: mMX
- Length: 1 tryte
- Description: X = -X; flip all trits in X. Same opcode as "NOT X".
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### ZERO X
- Opcode: m0X
- Length: 1 tryte
- Description: X = 0; short for "SET X, 0", which would be a 2 Tryte instruction "maX 000"
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### Logic operations

#### CMP X, Y
- Opcode: cXY
- Length: 1 tryte
- Description: compare register X and Y; compare flag is set to sign(X - Y).
- Compare flag: Set to sign(X - Y).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### CMP X, t9
- Opcode: mcX t9
- Length: 2 trytes
- Description: compare flag is set to sign(X - t9).
- Compare flag: Set to sign(X - t9).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### CPZ X
- Opcode: mCX
- Length: 1 tryte
- Description: Short form of "CMP X, 0".
- Compare flag: Set to sign(X).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### AND X, Y
- Opcode: bXY
- Length: 1 tryte
- Description: Perform tritwise ternary logical AND on registers X and Y. In ternary logic, - is FALSE, 0 is UNKNOWN/MAYBE, + is TRUE. Each pair of trits is compared, and their minimum value is used to construct the result, which is placed in register X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X & Y).
- Stack flag: No effect.

#### AND X, t9
- Opcode: mbX t9
- Length: 2 trytes
- Description: X &= t9, where & is the ternary AND operator.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X & t9).
- Stack flag: No effect.

#### OR X, Y
- Opcode: BXY
- Length: 1 tryte
- Description: Perform tritwise ternary logical OR on registers X and Y. In ternary logic, - is FALSE, 0 is UNKNOWN/MAYBE, + is TRUE. Each pair of trits is compared, and their maximum value is used to construct the result, which is placed in register X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X | Y).
- Stack flag: No effect.

#### OR X, t9
- Opcode: mBX t9
- Length: 2 trytes
- Description: X |= t9, where | is the ternary OR operator.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X | t9).
- Stack flag: No effect.

#### STAR X, Y
- Opcode: kXY
- Length: 1 tryte
- Description: Tritwise multiply registers X and Y. Useful for singling out particular trits. For example mmm * 00a = 00a.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X * Y).
- Stack flag: No effect.

#### STAR X, t9
- Opcode: mkX t9
- Length: 2 trytes
- Description: X *= t9, where * is the ternary STAR operator, tritwise multiplication.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: Set to sign(X * t9).
- Stack flag: No effect.

#### NOT X
- Opcode: mMX
- Length: 1 tryte
- Description: Perform tritwise ternary logical NOT on register X. NOT(TRUE) = FALSE, NOT(UNKNOWN) = UNKNOWN, NOT(FALSE) = TRUE. Equivalent (same opcode) to FLIP X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### Control flow operations

#### NOP
- Opcode: j00
- Length: 1 tryte
- Description: Do nothing except increment program counter (PC).
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### SLEEP/WAIT
- Opcode: M00
- Length: 1 tryte
- Description: CPU goes to sleep - will awake when external device tells it to. Useful to prevent data races when disks load/save.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### HALT
- Opcode: 000
- Length: 1 tryte
- Description: Halt and catch fire. Computer will stop.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### The ternary jump instruction

#### JP addr1/null, addr2/null, addr3/null

JP is the mnemonic for the _ternary jump instruction_, where 

- if compare flag < 0, jump to addr1 (or do nothing except to go to next instruction if null)
- if compare flag == 0, jump to addr2 (or do nothing if null)
- if compare flag > 0, jump to addr3 (or do nothing if null).

Jump opcodes may be longer than a single Tryte - in general, they have the following pattern. The first tryte is always of the form:

- XXXYYYZZZ

where XXX is +0+ , the septavingtesmal value 'j'; YYY is a sequence of three trits for the three arguments of JP, where

- if Y is +, this argument is an indirect register \[P\]
- if Y is 0, this argument is null
- if Y is -, this argument is an immediate address $Q.

The final three trits ZZZ are dependent on the number of register arguments:
- If there are no register arguments, ZZZ is either '0' or 'j':
    * '0' if the address arguments do not match, i.e. the general case. "JP $X, *, $Y" assembles to "jJ0 $X $Y"
    * 'j' if all the address arguments match. In this case, a shorthand is possible: "JP $X, *, $X" assembles to "jJj $X".
- A slightly special case applies for YYY = '0' - i.e. if all three arguments are null. In this case the instruction would be "JP *, *, *", a no-op. Different values of ZZZ then encode various special operations, namely JPS, PJP, and NOP.
- If there is a single register argument, the trits ZZZ encode the register used in the operation. For example, for the instruction "JP \[X\], $Y, $Z" the opcode is constructed in the following way:
    * XXX = 'j' as this is a jump instruction
    * YYY = +-- = 'e' as we have one register and two address arguments
    * ZZZ = the register code for the register X
therefore, for "JP \[B\], $000, $aaa" we have the assembled instruction "jeb 000 aaa".
- If there are two register arguments, then the value of ZZZ depends on whether those two registers match or not.
    * If they match, then ZZZ encodes the register used in the operation, and the opcode is followed by an address argument, if it exists. For example, "JP \[A\], $X, \[A\]" assembles to "jga $X".
    * If they don't match, then ZZZ = '0', and the opcode tryte is followed by a special 'register' tryte, which holds the registers used in the order they occurred in the jump instruction.
    * For example, "JP \[A\], \[B\], $X" assembles to "jk0 ab0 $X". "JP \[G\], $X, \[E\]" assembles to "jg0 g0e $X".
- If there are three register arguments:
    * If all three registers match - ZZZ encodes the register used. For example, "JP \[A\], \[A\], \[A\]" (which can be written as "JP \[A\]") assembles to "jma".
    * Otherwise, ZZZ = '0', and the registers are encoded in a 'register' tryte, following the opcode. For example, "JP \[D\], \[A\], \[F\]" assembles to "jm0 daf".

Special macro mnemonics are available for common forms of JP - they are described, along with their opcodes, below. The JP instruction has no effect on any flags - the compare flag is NOT reset after it is used.

#### JPZ $X - JP null, $X, null
- Opcode: jCj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag == 0.

#### JPZ \[X\] - JP null, \[X\], null
- Opcode: jcX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag == 0.

#### JPNZ $X - JP $X, null, $X
- Opcode: jJj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag != 0.

#### JPNZ \[X\] - JP \[X\], null, \[X\]
- Opcode: jjX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag != 0.

#### JPP $X - JP null, null, $X
- Opcode: jAj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag > 0.

#### JPP \[X\] - JP null, null, \[X\]
- Opcode: jaX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag > 0.

#### JPNP $X - JP $X, $X, null
- Opcode: jLj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag < 0 or == 0.

#### JPNP \[X\] - JP \[X\], \[X\], null
- Opcode: jlX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag < 0 or == 0.

#### JPN $X - JP $X, null, null
- Opcode: jIj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag > 0.

#### JPN \[X\] - JP \[X\], null, null
- Opcode: jiX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag > 0.

#### JPNN $X - JP null, $X, $X
- Opcode: jDj $X
- Length: 2 trytes
- Description: jump to address $X if compare flag < 0 or == 0.

#### JPNN \[X\] - JP null, \[X\], \[X\]
- Opcode: jdX
- Length: 1 tryte
- Description: jump to address stored in register X if compare flag < 0 or == 0.

#### JP $X
- Opcode: jMj $X
- Length: 2 trytes
- Description: unconditionally jump to $X

#### JP \[X\]
- Opcode: jmX
- Length: 1 tryte
- Description: unconditionally jump to address stored in register X.

#### JPS $X
- Opcode: j0J $X
- Length: 2 trytes
- Description: store the current program counter (+2 to jump past this instruction!) and jump to address $X. Often used to enter functions.

#### JPS \[X\]
- Opcode: j0X
- Length: 1 tryte
- Description: store the current program counter (+2 to jump past this instruction!) on the stack and jump to address stored in register X. Often used to enter functions.

#### PJP
- Opcode: j0j
- Length: 1 tryte
- Description: pop a Tryte off the stack, and jump to the recovered address. Often used to return from functions.