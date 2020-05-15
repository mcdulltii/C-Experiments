# Load

## Solving

- call sym dbp() is a function that detects breakpoints when the binary is run within a debugger

### ![Decompiled binary](img/5.png)

- Overwrite jump instruction to skip GDB check

### ![Overwrite ASM Instruction](img/6.png)

- Use gdb to step through instructions till string compare function is called

- Stack is pushed to save the entire decoded flag when the loaded strcmp function is run, thus obtaining the flag

### ![Flag in Stack](img/7.png)

## Workflow

- Strings are obfuscated via XOR, thus hidden from strings

### ![Obfuscation technique](img/1.png)

- Compile library within /tmp/ file to hide files during runtime

### ![Creation, Load, and Removal of Compiled Libary](img/2.png)

- Load function within library

### ![Loaded Function Within Compiled Library](img/3.png)

- Call loaded function to validate input string

### ![Call Loaded Function](img/4.png)
