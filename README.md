# CosmoVM
Assembler + emulator for an imaginary computer<br/>
Example assembly code in [/example/boot.asm](/example/boot.asm)

## Specifications
```c
/**
 * Cosmo 16-bit CPU
 * General purpose registers:
 * AZ, BZ, CZ, DZ, EZ, FZ, GZ, HZ
 *
 * Special registers:
 * MO: Memory Offset
 * XA: eXecution Address
 * SB: Stack base
 * SP: Stack ptr
 *
 * Flags from LSB to MSB
 * ERROR
 * EXCEPTION
 * EQUAL
 * GREATER
 * LESSER
 * RESET
 * SHUTDOWN
*/
/**
 * Instruction set:
 * i-variants: takes a immediate or a label name
 * b-variants: byte-wide as opposed to word-wide by default
 *
 * WCYL, Wait cycle does nothing
 * ADD ADDi, Add, result in first operand
 * AND ANDi, Logical AND, result in first operand
 * CALL, Store MO, XA in stack, jumps to code offset + first operand(IMM)
 * CLER, Clear error flag
 * CLXP, Clear exception flag
 * CMP CMPi, Sets condition flags accordingly
 * DEC, Decrement inplace register
 * DIV DIVi, Divide, quotient in IZ, remainder in JZ
 * IN, [!] first operand(REG) is destination, second operand(IMM) is port number
 * INC, Increment inplace register
 * --------------------
 * Following instructions jump to code offset + first operand(IMM)
 * JE, Jump if equal flag set
 * JER, Jump if error flag set
 * JG, Jump if greater flag set
 * JGE, Jump if greater flag set or equal flag set
 * JL, Jump if lesser flag set
 * JLE, Jump if lesser flag set or equal flag set
 * JMP, Jump unconditionally
 * JNE, Jump if equal flag cleared
 * JNER, Jump if error flag cleared
 * JNXP, Jump if exception flag cleared
 * JXP, Jump if exception flag set
 * LOP, Loop JZ times
 * LOPE, Loop if equal
 * LOPNE, Loop if not equal
 * --------------------
 * --------------------
 * Following instructions copy the second operand to the first operand
 * Move word
 * MOV, register <- register
 * MOVi, register <- immediate
 * LOAD, register <- memory at register
 * LOADi, register <- memory at immediate
 * STOR, memory at register <- register
 * STORi, memory at register <- immediate
 * COPY, memory at register <- memory at register
 * COPYi, memory at register <- memory at immediate
 *
 * Move byte
 * LOADb, register <- memory at register
 * LOADbi, register <- memory at immediate
 * STORb, memory at register <- register
 * STORbi, memory at register <- immediate
 * COPYb, memory at register <- memory at register
 * COPYbi, memory at register <- memory at immediate
 * --------------------
 * MUL MULi, Multiply, result in first operand
 * NEG, Negate inplace
 * NOT, Logical NOT inplace
 * OR ORi, Logical OR, result in first operand
 * OUT, [!] first operand(REG) is data, second operand(IMM) is port number
 * POP, Get value from stack to first operand(REG)
 * POPF, Get flags from stack
 * PUSH, PUSHi, Send first operand
 * PUSHF, Send flags to stack
 * RET, Get ZZ, YZ from stack
 * SHL, SHLi, Shift Left first operand(REG) is target, second operand(REG) is n-places
 * SHR, SHRi, Shift Right first operand(REG) is target, second operand(REG) is n-places
 * STER, Set error flag
 * STXP, Set exception flag
 * STRS, Set reset flag
 * STSD, Set shutdown flag
 * SUB SUBi, Substract, result in first operand
 * WCYL, Wait cycle, do nothing
 * XOR XORi, Logical XOR, result in first operand
*/
/**
 * Memory addresses
 * 0x0000 BOOT
 * 0xB4FF VIDEO
*/
```
## Port numbers
```c
/**
 * CosmoClock
 * 0x31: Get year
 * 0x32: Get month
 * 0x33: Get day
 * 0x34: Get hour
 * 0x35: Get minutes
 * 0x36: Get seconds
 *
 * CosmoScreen
 * 0x44: Set video mode, 0: Text, 1: Graphics
 *
 * CosmoKeyboard
 * 0x51: Set key selector, SDL Scancodes
 * 0x52: Get selected key state, 0: Up, 1: Pressed
 * 0x53: Get pressed key, SDL Scancodes
 *
 * CosmoDisk
 * 0x61: Set mode, 0: Read, 1: Write
 * 0x62: Select sector
 * 0x63: Load buffer/ Write buffer
 * 0x64: Get character/Put character
 * 0x65: Get sector count
 * 0x66: At the end of buffer
*/
```

## Dependencies
Used libraries: SDL2 https://www.libsdl.org/ <br/>
Used font("repo:/vgafont.ttf"): PCSenior font from http://www.zone38.net/