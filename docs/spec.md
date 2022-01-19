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
- Memory bank system allowing for up to 19,683 memory banks of storage, each with 6,561 Trytes
- I/O faciliated by dedicated PORT/IN/OUT instructions, allowing for 19,683 connected devices (some of which are used internally by TRIUMPH)

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

in memory, also have the PC (program counter), the SP (stack pointer), the MB (memory bank), and the current port PORT.

## Instruction set

The following is a list of ternary assembly instructions accepted by TRIUMPH. Each instruction has a unique 1-tryte opcode, with these opcodes being denoted ".XY" (for a 2-register instruction), "..X" (for a 1-register instruction) or "...", where '.' is a valid septavingtesmal character (M-A,0,a-m), and X/Y are 3-trit opcode representations of specific registers, given by the following table:

A - 00+ ('a')
B - 0+- ('b')
C - 0+0 ('c')
D - 0++ ('d')
E - +-- ('e')
F - +-0 ('f')
G - +-+ ('g')
H - +0- ('h')
I - +00 ('i')

For example, the general form of the opcode for "ADD X, Y" is eXY - "ADD A, B" is given by "eab". If instructions require raw memory addresses, these will immediately follow the opcode. For example, the jump instruction "JP $X" assembles to "jj0 X", where X is the raw tryte address of $X.

The instructions also permit indirect addressing of memory using registers, denoted by square brackets \[\] - for example, if register A = a00, \[A\] will point to the contents of memory at address $a00. The placeholder 't9' represents a 9-trit immediate value, standing for any value between -9,841 and 9,841.

### LOAD instructions

#### LOAD $X, Y
- Opcode: mdY $X
- Length: 2 trytes
- Description: Load register Y with contents of address $X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### LOAD \[X\], Y
- Opcode: dXY
- Length: 1 tryte
- Description: Load register Y with contents of indirect address given by register X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

### SAVE instructions

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

### Memory bank management

#### BANK X
- Opcode: mjX
- Length: 1 tryte
- Description: Set current BANK to value of register X

#### BANK t9
- Opcode: Mj0 t9
- Length: 1 tryte
- Description: Set current BANK to value of immediate t9

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
- Description: set register X equal to immediate value t9.
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
- Opcode: Mff
- Length: 1 tryte
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

### I/O operations

#### IN X
- Opcode: miX
- Length: 1 tryte
- Description: receive a single tryte from the currently connected PORT, and write it to register X.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### OUT X
- Opcode: mIX
- Length: 1 tryte
- Description: Send a single tryte, the value of register X, down the currently connected PORT.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### PORT X
- Opcode: mJX
- Length: 1 tryte
- Description: Set current PORT to value of register X

### PORT t9
- Opcde: MJ0 t9
- Length: 2 trytes
- Description: Set current PORT to value of immediate t9

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

#### SH X, Y
- Opcode: CXY
- Length: 1 tryte
- Description: Trit-shift the value of register X left or right by the value of the three least-significant trits of register Y. A positive number means shift left, i.e. multiply by 3 repeatedly. Negative means shift right. If Y == 0, nothing happens.
- Compare flag: No effect.
- Carry flag: If result overflows, carry flag will be set to the sign of register X before shifting (shifting preserves sign). If no overflow, carry flag will be set to '0'.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### SH X, t9
- Opcode: mCX t9
- Length: 2 trytes
- Description: Trit-shift the value of register X left or right by the value of the three least-significant trits of immediate value t9. A positive number means shift left, i.e. multiply by 3 repeatedly. Negative means shift right. SH X, 0 does nothing.
- Compare flag: No effect.
- Carry flag: If result overflows, carry flag will be set to the sign of register X before shifting (shifting preserves sign). If no overflow, carry flag will be set to '0'.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### INC X
- Opcode: mhX
- Length: 1 tryte
- Description: X += 1.
- Compare flag: No effect.
- Carry flag: If result overflows, carry flag will be set to the sign of register X before shifting (shifting preserves sign). If no overflow, carry flag will be set to '0'.
- Sign flag: Set with sign of result.
- Stack flag: No effect.

#### DEC X
- Opcode: mHX
- Length: 1 tryte
- Description: X -= 1.
- Compare flag: No effect.
- Carry flag: If result overflows, carry flag will be set to the sign of register X before shifting (shifting preserves sign). If no overflow, carry flag will be set to '0'.
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

#### AND X, Y
- Opcode: bXY
- Length: 1 tryte
- Description: Perform tritwise ternary logical AND on registers X and Y. In ternary logic, - is FALSE, 0 is UNKNOWN/MAYBE, + is TRUE. Each pair of trits is compared, and their minimum value is used to construct the result, which is placed in register X.
- Compare flag: Set to sign(X & Y).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### AND X, t9
- Opcode: mbX t9
- Length: 2 trytes
- Description: X &= t9, where & is the ternary AND operator.
- Compare flag: Set to sign(X & t9).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### OR X, Y
- Opcode: BXY
- Length: 1 tryte
- Description: Perform tritwise ternary logical OR on registers X and Y. In ternary logic, - is FALSE, 0 is UNKNOWN/MAYBE, + is TRUE. Each pair of trits is compared, and their maximum value is used to construct the result, which is placed in register X.
- Compare flag: Set to sign(X | Y).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### OR X, t9
- Opcode: mBX t9
- Length: 2 trytes
- Description: X |= t9, where | is the ternary OR operator.
- Compare flag: Set to sign(X | t9).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### STAR X, Y
- Opcode: GXY
- Length: 1 tryte
- Description: Tritwise multiply registers X and Y. Useful for singling out particular trits. For example mmm * 00a = 00a.
- Compare flag: Set to sign(X * Y).
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### STAR X, t9
- Opcode: mGX t9
- Length: 2 trytes
- Description: X *= t9, where * is the ternary STAR operator, tritwise multiplication.
- Compare flag: Set to sign(X * t9).
- Carry flag: No effect.
- Sign flag: No effect.
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
- Opcode: M00
- Length: 1 tryte
- Description: Do nothing except increment program counter (PC).
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### HALT
- Opcode: MMM
- Length: 1 tryte
- Description: Halt and catch fire. Computer will stop.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPZ \[X\]
- Opcode: jiX
- Length: 1 tryte
- Description: check compare flag, and jump to address $X if compare flag == 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPZ $X
- Opcode: jji $X
- Length: 2 trytes
- Description: check compare flag, and jump to address $X if compare flag == 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPNZ \[X\]
- Opcode: jIX
- Length: 1 tryte
- Description: check compare flag, and jump to address $X if compare flag != 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPNZ $X
- Opcode: jjI $X
- Length: 2 trytes
- Description: check compare flag, and jump to address $X if compare flag != 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPP \[X\]
- Opcode: jaX
- Length: 1 tryte
- Description: check compare flag, and jump to address $X if compare flag > 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPP $X
- Opcode: jja $X
- Length: 2 trytes
- Description: check compare flag, and jump to address $X if compare flag > 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPN \[X\]
- Opcode: jAX
- Length: 1 tryte
- Description: check compare flag, and jump to address $X if compare flag < 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPN $X
- Opcode: jjA $X
- Length: 2 trytes
- Description: check compare flag, and jump to address $X if compare flag < 0.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JP \[X\]
- Opcode: j0X
- Length: 1 tryte
- Description: Jump to address $X unconditionally.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JP $X
- Opcode: jj0 $X
- Length: 2 trytes
- Description: Jump to address $X unconditionally.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

#### JPS \[X\]
- Opcode: jfX
- Length: 1 tryte
- Description: Push the program counter onto the stack, and then jump to address $X. Used by function calls.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after decrementing, SP < mMM, stack flag will be set to '+', indicating overflow.

#### JPS $X
- Opcode: jjf $X
- Length: 2 trytes
- Description: Push the program counter onto the stack, and then jump to address $X. Used by function calls.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after decrementing, SP < mMM, stack flag will be set to '+', indicating overflow.

#### PJP
- Opcode jjF
- Length: 1 tryte
- Description: Pop an address off the stack, and then jump to that address.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: If after incrementing, SP > m00, stack flag will be set to '-', indicating underflow.

#### TJP $X, $Y, $Z
- Opcode: jjj $X $Y $Z
- Length: 4 trytes
- Description: Ternary jump instruction. If compare flag < 0, jump to $X; if compare flag == 0, jump to $Y; if compare flag > 0, jump to $Z.
- Compare flag: No effect.
- Carry flag: No effect.
- Sign flag: No effect.
- Stack flag: No effect.

## Memory map

TRIUMPH can address Tryte memory in the range $MMM-$mmm.
By default, stack pointer SP begins at $m00.

$MMM-$Emm : 6561 trytes of general purpose memory (bank switchable)
$DMM-$dmm : 6561 trytes of general purpose memory
$eMM-$mMM : PROGRAM memory; assembled code is written here. Use at own risk.
$mMM-$m0A : stack; stack pointer SP begins at $m00 and decrements as stack grows
$m00-$mmi : reserved for system use (boot code, etc)
$mmj      : current PORT
$mmk      : current BANK
$mml      : stack pointer SP
$mmm      : program counter PC