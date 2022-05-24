# Improved ternary jump instruction

27 instructions of the form
JP addr1/null, addr2/null, addr3/null

where if compare flag < 0, jump to addr1 (or do nothing if null), or if compare flag == 0, jump to addr2 (or do nothing if null), and if compare flag > 0, jump to addr3 (or do nothing if null).

Following is a proposed list of opcodes. They follow a pattern - an opcode, and then a list of addresses. In the opcode, there are three chars - these correspond to the type of the arguments. J if an address $X, 0 if null, or a-i if a register (denoted by X in the list below.) This pattern doesn't work for TJP \[X\], \[Y\], \[Z\], which would correspond to the opcode XYZ, but this clashes with many other opcodes (for example, TJP \[A\], \[B\], \[C\] corresponds to "abc", which is occupied by the SET B, C instruction.) This special case is accommodated by the opcode jjj, so the above instruction maps to "jjj abc". Lower case j also corresponds to special jump instructions.

# Opcodes

## JP $X, $Y, $Z
- Opcode: JJJ $X $Y $Z
- Length: 4 trytes
- Comment: equivalent to original "TJP $X, $Y, $Z"

## JP $X, $Y, *
- Opcode: JJ0 $X $Y
- Length: 3 trytes

## JP $X, $Y \[Z\]
- Opcode: JJZ $X $Y
- Length: 3 trytes

## JP $X, *, $Y
- Opcode: J0J $X $Y
- Length: 3 trytes

## JP $X, *, *
- Opcode: J00 $X
- Length: 2 trytes
- Comment: equivalent to "JPN $X"

## JP $X, *, \[Y\]
- Opcode: J0Y $X
- Length: 2 trytes

## JP $X, \[Y\], $Z
- Opcode: JYJ $X $Z
- Length: 3 trytes

## JP $X, \[Y\], *
- Opcode: JY0 $X
- Length: 2 trytes

## JP $X, \[Y\], \[Z\]
- Opcode: JYZ $X
- Length: 2 trytes

## JP *, $Y, $Z
- Opcode: 0JJ $Y $Z
- Length: 3 trytes

## JP *, $X, *
- Opcode: 0J0 $X
- Length: 2 trytes
- Comment: equivalent to "JPZ $X"

## JP *, $X, \[Y\]
- Opcode: 0JY $X
- Length: 2 trytes

## JP *, *, $X
- Opcode: 00J $X
- Length: 2 trytes
- Comment: equivalent to "JPP $X"

## JP *, *, *
- Opcode: 000
- Length: 1 tryte
- Comment: equivalent to NOP

## JP *, *, \[X\]
- Opcode: 00X
- Length: 1 tryte
- Comment: equivalent to JPP \[X\]

## JP *, \[X\], $Y
- Opcode: 0XJ $Y
- Length: 2 trytes

## JP *, \[X\], *
- Opcode: 0X0
- Length: 1 tryte
- Comment: equivalent to JPZ \[X\]

## JP *, \[X\], \[Y\]
- Opcode: 0XY
- Length: 1 tryte

## JP \[X\], $Y, $Z
- Opcode: XJJ $Y $Z
- Length: 3 trytes

## JP \[X\], $Y, *
- Opcode: XJ0 $Y
- Length: 2 trytes

## JP \[X\], $Y, \[Z\]
- Opcode: XJZ $Y
- Length: 2 trytes

## JP \[X\], *, $Y
- Opcode: X0J $Y
- Length: 2 trytes

## JP \[X\], *, *
- Opcode: X00
- Length: 1 tryte
- Comment: equivalent to JPN \[X\]

## JP \[X\], *, \[Y\]
- Opcode: X0Y
- Length: 1 tryte

## JP \[X\], \[Y\], $Z
- Opcode: XYJ $Z
- Length: 2 trytes

## JP \[X\], \[Y\], *
- Opcode: XY0
- Length: 1 tryte

## JP \[X\], \[Y\], \[Z\]
- Opcode: jjj XYZ
- Length: 2 trytes
- Comment: XYZ not acceptable.

# Extra commands

## JP $X
- Opcode: j00 $X
- Length: 2 trytes
- Comment: shorthand for JP $X, $X, $X - which would be 4 trytes (JJJ $X $Y $Z)

## JP \[X\]
- Opcode: j0X
- Length: 1 tryte
- Comment: again, convenient shorthand for JP \[X\], \[X\], \[X\], which would be 2 trytes (jjj XXX)

## JPS $X
- Opcode: jjf $X
- Length: 2 trytes
- Comment: unchanged from before

## JPS \[X\]
- Opcode: jfX
- Length: 1 tryte
- Comment: unchanged from before

## PJP
- Opcode: jjF
- Length: 1 tryte
- Comment: unchanged from before

# Old opcodes - are these necessary?

## JPNZ $X
- Opcode: jjI $X

## JPNZ \[X\]
- Opcode: jIX