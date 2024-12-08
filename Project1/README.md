
# Command Line Shell

**Contributors:**  
- Kyle Nwosu 

---

## Project Overview

This project implements a command-line shell in C that provides basic functionalities like executing commands, managing input/output redirection, handling pipes, and supporting background processes. It also demonstrates the use of system calls, process control, and signal handling to create an interactive shell environment.

---

## Features

- **Custom Command Prompt:** Displays the current working directory as a prompt.  
- **Built-in Commands:**
  - `cd`: Change directory.
  - `pwd`: Display the current working directory.
  - `echo`: Print arguments to the standard output.
  - `env`: List all environment variables.
  - `setenv`: Set environment variables.  
- **External Commands:** Executes external programs using `execvp()`.  
- **I/O Redirection:** Supports input (`<`) and output (`>`) redirection.  
- **Piping:** Allows the use of pipes (`|`) for inter-process communication.  
- **Background Processes:** Supports background execution with `&`.  
- **Signal Handling:** Gracefully handles `Ctrl-C` (`SIGINT`) and enforces a timeout for long-running processes (`SIGALRM`).  

---

## Design Choices

1. **Input Parsing:**  
   The shell tokenizes input using `strtok` and a set of delimiters, ensuring robust command parsing.  
2. **Process Management:**  
   Commands are executed in child processes using `fork()`, allowing the parent shell to remain interactive.  
3. **Signal Handling:**  
   - `SIGINT` is handled to prevent abrupt termination of the shell.  
   - `SIGALRM` is used to terminate unresponsive processes.  
4. **I/O Redirection and Pipes:**  
   Custom handling of file descriptors ensures seamless redirection and communication between commands in a pipeline.  
5. **Modular Design:**  
   Separate functions for built-in commands (`cd`, `pwd`, etc.) enhance code readability and maintainability.  

---

## Code Documentation

### Key Functions
- **`update_prompt()`:** Updates the shell prompt with the current directory.  
- **`handle_cd(char *directory)`:** Implements the `cd` command.  
- **`sigint_handler(int signum)`:** Handles `Ctrl-C` interruptions gracefully.  
- **`sigalrm_handler(int signum)`:** Terminates processes that exceed a timeout.  
- **`main()`:** The core shell loop that handles input, command execution, and process management.

### File Structure
- `shell.c`: Contains the source code for the shell.

---


### Compilation
Run the following command to compile the program:
```bash
gcc -o shell src/main.c
```

### Usage
Start the shell by running:
```bash
./shell
```

#### Examples:
1. **Change directory:**
   ```bash
   cd /path/to/directory
   ```
2. **Run a command in the background:**
   ```bash
   ls -l &
   ```
3. **Redirect output:**
   ```bash
   ls > output.txt
   ```
4. **Pipe commands:**
   ```bash
   ls | grep "main"
   ```

---
