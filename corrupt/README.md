# Dynamic

## Solving

- TBC

## Workflow

- Collapsed ELF Headers

- Memory needs to be virtually allocated between 0x100000 and 0x800000 for executable to run

- Part of _main function within ELF magic header, replacing `times 9 db 0`

### ![Collapsed Header](img/1.png)

- MPROTECT to change address permissions to readable and writable for stdin buffer to be read

### ![MPROTECT](img/2.png)

- Read from stdin and check if read

### ![Read STDIN](img/3.png)

- Compares key and input string with truncated length of key

### ![Strncmp](img/4.png)

- Uses tailer 2 bytes of input string of certain length as XOR and SUB keys for decoding of flag

### ![Decode Flag](img/5.png)

- Prints decoded flag string no matter correct or wrong

### ![Prints "Flag"](img/6.png)

- Stored string and hex array for key and flag respectively

- Tailing `db 0`s for key buffer within .rodata section to hold XOR and SUB characters for decoding of flag

- Hex array of encoded flag stored within .data section

### ![Stored Strings](img/7.png)
