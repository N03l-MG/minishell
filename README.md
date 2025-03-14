<div align="center">
  <h1>
    minishell (42 project)
  </h1>
  <p>
    <b><i>A simple bash-like shell written in C.</i></b>
  </p>
</div>

_This project was done in collaboration with my peer [Cimex](https://github.com/Cimex404). Make sure to check out his work!_

Our goals for minishell were to fulfil the basic requirements. Due to the scale of this project, we did not add bonus features such as handling `&&`, `||` or `;`. It should be noted that, unlike bash, minishell does not read a startup file and instead copies its environment directly from the env argument. Additionally features like backslashes and unclosed quotes/pipes are also not implemented. Any unclosed quotes or improper pipes/redirections will be handled as input or syntax errors respectively.

## Features
- **Built-Ins:**
	Some basic commands have been reimplemented as built-ins.
	- `cd` - Changes the current working directory. Takes relative or absolute paths as well as `~` and `-`.
	- `pwd` - Prints the current working directory.
	- `echo` - Prints a string or series of strings after it until a pipe or redirection. Can handle double and/or single quotes and environment variable expansion with `$`.
	- `env` - Prints the environment variables in the current runtime, excluding those with an empty value.
	- `export` - Takes one or multiple arguments to add variables to the current environment. Any invalid syntax will be ignored and when ran with no arguments it will print an alphabetically sorted list of all existing environment variables including those with an empty value.
	- `unset` - Takes one or multiple arguments to remove variables from the current environment. Much like export, invalid syntax or variables are ignored. Running with no arguments will do nothing.
	- `exit` - Exits the shell. Can take numeric or string arguments. If ran with no arguments, it will exit the shell with the last status received from an execution. If ran with any number, it will exit with said number cast as an unsigned byte (0-255). If ran with any string, it will default exit with 255.
- **Variable Expansion:**
	Using `$` before a string will automatically replace it with its corresponding value if it is an existing environment variable. This works with double quotes as well, merging multiple strings as a single token and performing expansion, and works with single quotes doing the same but ignoring the expansion. The last exit status can also be accessed with `$?`.
- **Signal Handling:**
	Multiple signals are handled and update the status accordingly. 
	- `Ctrl+\` has been disabled by default.
	- `Ctrl+D` types a null prompt which exits the shell. It can also be used to exit a heredoc early or close a pending cat, grep, wc or any other command dependent on input.
	- `Ctrl+C` Interrupts a currently running process and skips the current prompt.
- **Redirections:**
	All types of redirections from Bash have been implemented and behave identically.
	- `<` - Reads from an input file.
	- `>` - Writes to an output file, truncating existing content. If said file does not exist, it will create it.
	- `>>` - Behaves identically to the write redirection but appends content instead of truncating.
	- `<<` - Creates a temporary heredoc file and allows the user to write to it. The string immediately after the heredoc symbol will be used as a delimiter to close the heredoc. Expansion will still take place inside the heredoc but the delimiter string itself will be treated as raw characters without any expansion or quote processing.
- **Execution and Pipes:**
	The shell will run all installed and native Unix/Bash commands as well as accept direct paths to executables. It can handle any number of pipes (`|`) from one command to another, executing each one in its own child process. All of this can be combined with redirections and all parsing features mentioned prior, just like Bash.
- **Working History:**
	The up and down arrow keys can be used to cycle through past commands. All commands not interrupted by a signal are added to the history.
- **Error Handling:**
	All kinds of errors are handled with their respective status codes. As mentioned prior any incorrect quotes, pipes or redirections will be treated as either input or syntax errors. Additionally, memory is managed and safely handled to avoid segmentation faults or leaks.

## Installation
To install our minishell in your system you can run this command where you wish to clone the source:

```bash
git clone --recurse-submodules https://github.com/N03l-MG/minishell.git && cd minishell && ./mini_installer.sh
```

The shell executable will be in the directory `$HOME/bin` and added automatically to your .bashrc or .zshrc. Simply run `minishell` to use it!
DEPENDENCIES: c-compiler, make, git, bash (naturally).
NOTE: This installer does _not_ work for Fish! Additionally, the project depends on my [libft](https://github.com/N03l-MG/libft) which is added as a submodule.

## Tokenization Notes
Most complex shells use an abstract syntax tree (AST) to store the user input, which allows for the handling of operation precedence. In our case however, due to the absence of logical ANDs or ORs, we did not see this as a necessary parsing method. Instead we parse sequentially into a dynamic array of tokens and execute in order. While inflexible, this is very performant and simple compared to other kinds of parsing and data structures.

In the future I may go on to change the tokenization to use an AST and handle the bonus operators, but this depends entirely on my availability.

## Addendum
The project was carried out roughly in the span of a month and a half. Many systems were torn down and rebuilt from scratch and many features were modified to better match Bash. The project's subject is quite specific as to what it wants and thus we prioritized this over our own ideas. In the end we are satisfied with the final product, even if it may be quite improvable.
This project scored us 100% in all evaluations.
