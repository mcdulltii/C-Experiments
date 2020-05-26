# ProcHollow

## Workflow

- Similar to Rewrite

- Spawns child process to execv hello binary in pwd

- PTRACE_TRACEME for parent process to halt child process

### ![Fork process](img/3.png)

- Wait to read pid from pipe from child process

### ![Parent process](img/1.png)

- SIGTRAP child process to edit its virtual memory

- Calculate start and end of memory addresses with pwd as string search, to only find Code sections in virtual memory of child process

### ![Calculate addresses](img/2.png)

- Searches /proc/{pid}/maps by line

### ![Read /proc/{pid}/maps](img/4.png)

- Reads /proc/{pid}/mem from start address of Code section

- Locates main function byte sequence to find offset

- Replaces printf value with 0x10

### ![Rewrites /proc/{pid}/mem](img/5.png)
