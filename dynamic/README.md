# Dynamic

## Solving

- Flag is 35 characters long, from the self modification

- Flag is encoded using the "ic..." sequence as key

### ![Extract Sequence and Encoded Flag](./6.png)

- Extract 35 rows of \x__ characters

- Characters with 'c' as key are bit subtracted by 0x47, while those with 'i' remain the same

- Decode characters using the above rule to obtain the flag

### ![Decode Flag](./7.png)

## Workflow

- Change file permissions to RWX, then to X when exited

### ![Changing Binary Permissions](./4.png)

- Check if binary is run by a debugger, then change strlen check in main() from 20 to 35

### ![GDB Check](./1.png)

- Decode string using bit subtraction, then call DC() to load function dynamically

### ![Decode String and Call Loaded Function](./5.png)

- Finds function name within struct, and loads the function with the corresponding name

### ![Function loader](./3.png)

- Call loaded function to validate input string using ty as the index

### ![Dynamically Loaded Function](./2.png)
