# Rewrite

## Solving

- Encoded strings loaded as int arrays

### ![Encoded Strings](./9.png)

- Encoded flag loaded as int array

### ![Encoded Flag](./10.png)

- Encoded strings can be split by the differences in opcodes
  - eg. c745________ is a different int array compared to c785________

- Encoded strings tailer are the NULL character, which are 0x1A for the strings, and 0x56 for the flag

- XOR or bit addition/subtraction can be used to identify which is the encoding method

- By trial and error, bit subtraction is found to be the decoding method, thus bit addition is the encoding method

### ![Decode Function](./11.png)

- Extract the last character before the \x00 characters from each instruction to obtain the characters to form each string

- Similar to the encoded strings, only the bit offset is different (0x56 instead of 0x1A), which obtains the flag

### ![Output](./12.png)

## Workflow

- Strings are encoded via bit addition

### ![Obfuscation technique](./1.png)

- Concatenate initial string s from char array

### ![Initial String](./2.png)

- Fork process to thread functions

- Calculate pid, address and length of initial string, within the virtual memory of the primary process

- Write to pipe to transfer values to secondary process

### ![Calculate Important Values](./3.png)

- Read from pipe and convert string values to integers

### ![Read Values from Primary Process](./4.png)

- Secondary process to read /proc/{pid}/maps of primary process to ensure the heap of the primary process has the correct readable and writable permissions

### ![Read /proc/{pid}/maps](./5.png)

- Secondary process to read /proc/{pid}/mem of primary process, then overwrite a new string onto the same address of the initial string

### ![Read and Write to /proc/{pid}/mem](./6.png)

- Primary process prints the initial string, then parses the input string into the next string compare function while delaying for the secondary process to overwrite the initial string

### ![Validation Function](./7.png)

- Validates input string with overwritten string

### ![Strcmp Function](./8.png)
