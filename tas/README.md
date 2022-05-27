# tas

tas is an assembler for the ternary computer TRIUMPH - it converts .tas files into .to (ternary object) files, ready for linking into a ternary machine code .tri file, or for writing to a ternary disk .td file.

This project uses the command line parser CLI11 v1.9.1 by Henry Schreiner at https://github.com/CLIUtils/CLI11 - full licence given in include/CLI11/CLI11.hpp.

## Design principles

- There should be a one-to-one mapping between assembly instructions and opcodes. Abstractions should be kept to a minimum.
- Fundamentally, we only need ways of organising blocks of instructions; { and } should begin and end blocks, and these blocks can have names. These names will be accepted as addresses in instructions (for example, JP)
- 'Calling' functions requires more conventions to be figured out, and we're not there yet, I've not written enough code. Try and keep it simple at first, and grow from there.
- Multiple files, imports, namespaces? Ignore these for now, focus on single files. Multiple files ought to assemble separately, and would probably need a 'use' statement to require other blocks - but this needs care to avoid circular imports...
- As a first pass, ignore these complications. Focus on implementing the basic set of instructions and the minimal amount of infrastructure and token types needed for that.
- Main objects:
    * Token: has a TokenType and the contents of the token.
    * Statement: a list of Tokens, a StatementType, and a line number.
    * Block: have a list of Statements, a name, and an address, which will be figured out at link time.