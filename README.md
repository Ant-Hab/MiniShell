# MiniShell

**MiniShell** is a simple shell implementation written in C as part of the 42 curriculum. It replicates basic Bash behavior—parsing user input, handling quotes, pipes, and redirections—built entirely from scratch using a custom Libft library.

## 🚀 Features

- **Prompt:** Display a prompt when waiting for a new command.
- **History:** Has a working history.
- **Search and Launch:** Search and launch the right executable (based on the `PATH` variable or using a relative or absolute path).
- **Signals:** Handles global variables to indicate received signals (e.g., `ctrl-C`, `ctrl-D`, `ctrl-\`).
- **Quotes:**
  - `'` (single quote): Prevents interpretation of meta-characters.
  - `"` (double quote): Prevents interpretation of meta-characters except for `$`. 
- **Redirections:**
  - `<` redirect input.
  - `>` redirect output.
  - `<<` heredoc (doesn't update history).
  - `>>` redirect output in append mode.
- **Pipes:** `|` The output of each command in the pipeline is connected via a pipe to the input of the next command.
- **Environment Variables:** Handle environment variables (`$`) and `$?`.
- **Builtins:**
  - `echo` with option `-n`
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options

## 🛠️ Usage

1. Compile the shell:
   ```bash
   make
   ```

2. Run the shell:
   ```bash
   ./minishell
   ```

3. Use it like a normal shell:
   ```bash
   minishell$ ls -la | grep "Makefile" > output.txt
   ```