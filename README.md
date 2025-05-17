# Mini Project 1: Building a Shell in C

## Reference 
https://karthikv1392.github.io/cs3301_osn/mini-projects/mp1
## Overview

## Demo Video
https://www.loom.com/share/95ebe0cf1d644dcc98c0b4f8d38fe960?sid=6e3b708e-ff85-4d0f-8d55-98cbadfa0fb9

This project involves building a custom shell in C, replicating core functionalities of a standard Unix shell. The shell is designed to handle various commands, system calls, and process management features. It is divided into two parts:

* **Part A: Basic System Calls** - Focuses on fundamental shell functionalities, including command parsing, directory navigation, file listing, process information, and command logging.
* **Part B: Processes, Files, and Misc.** - Extends the shell with advanced features like I/O redirection, pipes, signal handling, process management, and configuration file support.

## Project Structure

The codebase is structured into multiple C files for modularity, each handling specific functionalities. Header files are used to manage dependencies and declarations.

## Features

### Part A: Basic System Calls

* **Prompt Display:**
    * Displays a prompt with the current username, system name, and current directory.
    * Handles relative and absolute paths.
* **Input Handling:**
    * Parses commands separated by `;` or `&`.
    * Handles whitespace and tab characters.
    * Handles errors in commands.
* **`hop` Command:**
    * Changes the current directory.
    * Supports `.`, `..`, `~`, and `-` flags.
    * Handles both absolute and relative paths.
* **`reveal` Command:**
    * Lists files and directories in lexicographical order.
    * Supports `-a` and `-l` flags.
    * Color coded output.
* **`log` Command:**
    * Maintains a history of commands.
    * Supports `log purge` and `log execute <index>`.
    * Does not store the log command in the log.
* **System Commands:**
    * Executes other system commands in foreground and background.
    * Prints execution time for foreground processes exceeding 2 seconds.
    * Displays background process status upon completion.
* **`proclore` Command:**
    * Displays process information (PID, status, group, memory, path).
* **`seek` Command:**
    * Searches for files and directories.
    * Supports `-d`, `-f`, and `-e` flags.

### Part B: Processes, Files, and Misc.

* **.myshrc Configuration:**
    * Supports aliases and functions.
* **I/O Redirection:**
    * Handles `>`, `>>`, and `<` operators.
    * Creates output files if they do not exist.
* **Pipes:**
    * Supports multiple pipes (`|`).
* **Redirection with Pipes:**
    * Combines I/O redirection and pipes.
* **`activities` Command:**
    * Lists currently running processes spawned by the shell.
* **Signals:**
    * `ping <pid> <signal_number>` sends signals to processes.
    * Handles Ctrl+C, Ctrl+D, and Ctrl+Z.
* **`fg` and `bg` Commands:**
    * Manages foreground and background processes.
* **`neonate` Command (BONUS):**
    * Prints the PID of the most recently created process at set intervals.

## Compilation

To compile the shell, use the provided `Makefile`:

```bash
gcc *.c
```bash
./a.out