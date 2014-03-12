MISH is a fairly simple interactive UNIX shell implemented in C 
using the fork/exec pattern. It has a number of internal commands
that are handled by the shell itself, and any other command is dispatched
to the path and executed by the machine. If the command cannot be found 
or some other error occurs, the user is notified and execution continues
if it was not a fatal error. 

The internal commands include: 
verbose {on/off}:
	Turns verbose mode on or off. Verbose mode displays valuable information
	about each command as it executes (command name, token list, process ID)

help:
	Displays all available internal commands.

history:
	Displays all commands that have been executed in this instance of MISH.

quit:
	gracefully exits the program.

To launch from the command line:
mish {0/1} // 0 defaults verbose mode to on, 1 to off.

First, the program handles the command line args, turning verbose mode on or 
off accordingly. Then, it enters the main control loop. In the loop, it first 
processes the user input by parsing it into a list of tokens. Then it checks 
if the command was an internal command, responding appropriatley if it was. 
If not, the system assumes it was an external command and forks, creating a 
new process, and the child process calls execvp on the command. 
I used a top-down approach to the design of the program, starting with the 
user input and basic control loop, adding funtionality piece-by-piece (saving 
the annoying parts for last...)

