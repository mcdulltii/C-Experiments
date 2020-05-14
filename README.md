# C-Experiments
Experiments on C Exploits

## Dynamic (c)
- Loads string compare function on runtime
- Self modifies strlen comparison value
- Hides string using bit addition

## Load (cpp)
- Loads compiled library file on runtime
- Loads string compare function from loaded library
- Hides string using XOR from templates

## Rewrite (c)
- Spawns/Clones secondary process for self modification
- Secondary process modifies /proc/{pid}/mem of primary process
- Hides string using bit addition
