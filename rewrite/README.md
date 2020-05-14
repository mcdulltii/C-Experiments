# Workflow

- Strings are encoded via bit addition

## ![Obfuscation technique](./1.png)

- Concatenate initial string s from char array

## ![Initial String](./2.png)

- Fork process to thread functions

- Calculate pid, address and length of initial string, within the virtual memory of the primary process

- Write to pipe to transfer values to secondary process

## ![Calculate Important Values](./3.png)

- Read from pipe and convert string values to integers

## ![Read Values from Primary Process](./4.png)

- Secondary process to read /proc/{pid}/maps of primary process to ensure the heap of the primary process has the correct readable and writable permissions

## ![Read /proc/{pid}/maps](./5.png)

- Secondary process to read /proc/{pid}/mem of primary process, then overwrite a new string onto the same address of the initial string

## ![Read and Write to /proc/{pid}/mem](./6.png)

- Primary process prints the initial string, then parses the input string into the next string compare function while delaying for the secondary process to overwrite the initial string

## ![Validation Function](./7.png)

- Validates input string with overwritten string

## ![Strcmp Function](./8.png)
