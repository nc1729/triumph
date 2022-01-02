# TRIUMPH ternary computer

## Specifications

- little-endian ternary computer
- 9-trit data width and address space (capable of addressing 19,683 trytes)
- Memory banking allowing for up to 19,683 memory banks of storage, each with 6,561 Trytes
- I/O faciliated by dedicated PORT/IN/OUT instructions, allowing for 19,683 connected devices (some of which are used internally by TRIUMPH)

## Registers

9 9-trit registers:

A - general purpose (GP) register
B - GP register
C - GP register
D - GP register
E - GP register
G - GP register
H - GP register
R - GP register; return values are stored here
F - flags, from least signficant trit to most significant trit:
    COMPARE - stores result of CMP instruction
    CARRY - ADD/ADC instructions can produce carry

in memory, also have the PC (program counter), the SP (stack pointer), the MB (memory bank), and the current port PORT.

## Instructions

In these instructions, 'tX' represents a septavingtesmal number with X trits - for example, t9 is a 9-trit immediate value. The square brackets \[\] indicate the address of that value - for example, if A = a00, \[A\] will point to the contents of memory at $a00.

### LD A, \[t9/B\]
- Load register A with contents of address \[t9, B\].

### SV \[t9/A\], t9/B
- Save contents of immediate value t9 or register B into address \[t9, A\].

### SET A, t9/B
- Set register A equal to immediate value t9, or register B.

### SWAP A, B
- Swap the values of registers A and B.

### PUSH t9/A
- increment the stack pointer SP and write the contents of t9/A to that point in memory.
- may set stack status flag in case of overflow.

### POP A
- write the contents of address \[SP\] to register A, and then decrement SP.
- may set stack status flag in case of underflow.

### IN A
- read the contents of register A as input from the port PORT.

### OUT A
- write the contents of register A as output to the port PORT.

### CMP A, t9/B
- Compare the values of register A and t9/B. 
- If A > t9/B, COMPARE flag is set to +.
- If A == t9/B, COMPARE flag is set to 0.
- If A < t9/B, COMPARE flag is set to -.

### JP \[t9/A\]
- Jump unconditionally to the address \[t9/A\].

### JPZ \[t9/A\]
- Jump to address \[t9/A\] if COMPARE flag is zero.

### JPP \[t9/A\]
- Jump to address \[t9/A\] if COMPARE flag is +.

### JPN \[t9/A\]
- Jump to address \[t9/A\] if COMPARE flag is -.

### AND A, t9/B
- Apply the ternary logic AND operator tritwise to A and t9/B. The result is stored in A.
- In ternary logic, + (1) is TRUE, 0 is UNKNOWN, - (-1) is FALSE.
- Equivalent to applying min(A, B), trit-by-trit. For example, TRUE and UNKNOWN = UNKNOWN, and min(1, 0) = 0.

### OR A, t9/B
- Apply the ternary logic OR operator tritwise to A and t9/B. The result is stored in A.
- Equivalent to applying max(A, B), trit-by-trit.

### NOT A
- Apply the ternary logic NOT operator tritwise to A. The result is stored in A.
- In ternary logic, not TRUE = FALSE, not UNKNOWN = UNKNOWN, not FALSE = FALSE.
- Equivalent to setting A = -A.

### ADD A, t9/B
- Add register A to t9/B. The result is stored in A. Equivalent to A -> A + B.
- Will overflow is result is not in the range \[-9841, 9841\] - in this case, the CARRY flag is set.
- No carry is used for this operation.

### ADC A, t9/B
- Add register A to t9/B, with the CARRY flag. The result is stored in A. Equivalent to A = A + B + c.
- Will overflow is result is not in the range \[-9841, 9841\] - in this case, the CARRY flag is set.

### INC A
- Increment the value of A. A -> A + 1.
- Will overflow if A was equal to 9841 = $mmm - result of A in this case will be -9841 = $MMM.
- This overflow is silent - CARRY flag will NOT be set.

### DEC A
- Decrement the value of A; A -> A - 1.
- Will underflow if A was equal to -9841 = $MMM - result of A in this case will be 9841 = $mmm.
- This underflow is silent - CARRY flag will NOT be set.

### SH A, t3/B
- Trit-shift the value of A left by t3/B trits. Equivalent to A -> (3**(t3)) * A
- If t3/B < 0, the value of A is trit-shifted right.
- Only the least three significant trits of B are used.
- If abs(t3/B) = 10, 11 or 12, this instruction is equivalent to SET A, 0.

### NOP
- Do nothing for 1 CPU cycle.

## Opcodes

Opcodes are usually of the form:
    xxxyzzyzz
where
    xxx is the 3-trit identifier unique to each instruction
    zz is the register identifier, if used
    y is + if the following two trits are a register, - if this argument is an immediate (which will follow in the next Tryte), 0 if the opcode ends.

### Instruction 3-trit identifiers

LD    : 00+ : a
SV    : 00- : A
SET   : 0+- : b
SWAP  : 0-+ : B
PUSH  : 0++ : e
POP   : 0-- : E
IN    : +00 : i
OUT   : -00 : I
CMP   : 0+0 : c
JP    : +0+ : j
JPZ   : -0- : J
JPP   : ++- : k
JPN   : --+ : K
AND   : ++0 : l
OR    : --0 : L
NOT   : --- : M
ADD   : +-+ : g
ADC   : -+- : G
INC   : +0- : h
DEC   : -0+ : H
SH    : 0-0 : C
NOP   : 000 : 0

### Register identifiers

A     : 00
B     : 0+
C     : 0-
D     : +0
E     : -0
G     : +-
H     : -+
R     : --
F     : ++

### Examples

SET A, B:
SET -> 0+- : b
A   -> 00
B   -> 0+
Both registers are used, so both y-trits are +
    SET A, B -> 0+- + 00 + 0+ -> bij

ADD E, 100:
ADD -> +-+ : g
E   -> -0
100 is immediate, so second y-trit is -, 100 is in the next Tryte (as 0dH = 000 0++ -0+)
    ADD E, 100 -> +-+ + -0 - 00 | 000 0++ -0+ -> gfI 0dH

## Memory map

TRIUMPH can address Tryte memory in the range $MMM-$mmm.
By default, stack pointer SP begins at $m00, so PUSHing to the stack will eventually grow it into negative memory as the stack pointer becomes 'negative'.

$MMM-$M00 : negative stack (recommended)
$M0a-$00A : general purpose memory
$000-$i0A : general purpose memory (bank switchable)
$i00       : program counter (PC)
$i0a       : stack pointer (SP)
$i0b       : memory bank (MB)
$ioc       : current port (PORT)
$iod-$m0A : general purpose memory
$m00-$mmm : positive stack (recommended)

## Macros

### PORT t9/A
- Set the currently accessible port; the PORT value can be between -9841 and 9841 (the range of a 9-trit signed integer.)
- Assembles to SV t9/A, $ioc

### BANK t9/A
- Set the currently accessible memory bank; the BANK value can be between -9841 and 9841.
- Assembles to SV t9/A, $iob