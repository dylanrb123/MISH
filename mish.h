// Description: header for main mish file
// @author: Dylan Bannon


#ifndef MISH_H
#define MISH_H

/// Max input line length
#define MAX_LINE 100

/// deliminators for strtok
#define DELIMS " \t\r\n"

/// number of internal commands
#define NUM_CMDS 4

const char* const INTERNAL_CMDS[] = {"verbose", "help", "history", "quit"};

typedef struct {
  char** tokens;
  int len;
} TokenStruct;


/// Checks if an entered command is an internal command
///   @param cmd c-string to check if is command
///   @return 0 if not an internal command, else non-zero
///   @returns 0 on error, else nonzero
int is_internal_cmd(char* cmd);

/// tokenizes the c-string input, dynamically allocating an array to store the tokens.
/// Reallocates if there more than tokens.
///   @param input the string to tokenize
///   @return a pointer to a TokenStruct containing the tokens and 
TokenStruct* tokenize(char* input);

/// Takes a pointer to a TokenStruct and calls the proper internal command
///   @param tokens the struct containing and array of the tokens and number of tokens
int process_internal_cmd(TokenStruct* tokens);

/// Turn the shell verbose on or off based on the argument passed to the verbose command.
/// When verbose is turned on, the shell should print a table/list as shown in the
/// examples. After the command executes, the verbose mode should print the execution
/// status value returned to the shell by the command. 
///   @param argc the number of arguments
///   @param argv the arguments
///   @returns nonzero on error
int verbose(int argc, char** argv);

/// Print a list of internal commands, and continue the cycle of prompt -- get command --
/// execute command.
///   @param argc the number of arguments
///   @param argv the arguments
///   @returns nonzero on error
int help(int argc, char** argv);

/// Print a list of commands executed so far, including their arguments. The history list
/// should be numbered starting from the value 1.
///   @param argc the number of arguments
///   @param argv the arguments
///   @returns nonzero on error
int history(int argc, char** argv);

/// Clean up memory and gracefully terminate mish.
///   @param argc the number of arguments
///   @param argv the arguments
///   @returns nonzero on error
int quit(int argc, char** argv);

#endif

