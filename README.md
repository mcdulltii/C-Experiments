# C-Experiments

Experiments on C Exploits

## Environment

- 64 bit Linux Systems

## Completed Challenges

### - Dynamic (c)

- Loads string compare function on runtime
- Self modifies strlen comparison value
- Hides string using bit addition

### - Load (cpp)

- Loads compiled library file on runtime
- Loads string compare function from loaded library
- Hides string using XOR from templates

### - Rewrite (cc)

- Spawns/Clones secondary process for self modification
- Secondary process modifies /proc/{pid}/mem of primary process
- Hides string using bit addition

## Extras

### - Screwed (c)

- Corrupts ELF 64-bit or 32-bit headers with 0xffff values for e_shoff, e_shnum and e_shstrndx
- Binary is still able to be run normally, but crashes when debugged
