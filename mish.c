// Description: main control loop for interactive shell
// @author: Dylan Bannon

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "mish.h"

static int verbose_mode = 0;
static char history_array[100][1000];
static int numHist = 0;

/// main function, has input loop and handles forking/executing
int main(int argc, char* argv[]) {

  if(argc > 1) {
    char* sym;
    verbose_mode = strtol(argv[1],&sym,10);
    if(*sym) {
      perror("Error");
    }
  }
  
  int sequenceNum = 1;
  char* inputStr = NULL;
  size_t bytesRead = 0;
  size_t nbytes = MAX_LINE;
  

  while(69) {
    printf("mish[%d]> ",sequenceNum);
    bytesRead = getline(&inputStr, &nbytes, stdin);
    

    if(bytesRead == -1) {    // Error condition of geline
      putchar('\n');
      free(inputStr);
      exit(EXIT_SUCCESS);
    } 
    
    else if(bytesRead == 1) {    // empty input
      continue;
    } 
    
    else {    // process the commands
      sequenceNum++;
      TokenStruct* tokens = tokenize(inputStr);

      strcpy(history_array[numHist],inputStr);
      numHist++;
          
      // verbose mode stuff
      if(verbose_mode) {
        printf("\tcommand: %s\n",inputStr);
        printf("\tinput command tokens:\n");
        for(int i=0;i<tokens->len-1;i++) {
          printf("\t%d: %s.\n",i,tokens->tokens[i]);
        }
      }
      
      if(is_internal_cmd(tokens->tokens[0])) {    // Check if it's an internal command
        if(strcmp(tokens->tokens[0],"quit") == 0) free(inputStr);
        process_internal_cmd(tokens);

      } else {    //Handle external commands
        pid_t id,my_id;
        int status;
        char* cmd_array[tokens->len + 1];
        
        
        id = fork();
        switch(id) {

        case -1:
          // error forking
          perror("fork");
          free(inputStr);
          exit(EXIT_FAILURE);

        case 0: 
          // child process
          my_id = getpid();
          
          if(verbose_mode) {
            printf("\twait for pid %d: %s\n",my_id,tokens->tokens[0]);
            printf("\texecvp: %s\n",tokens->tokens[0]);
          }

          // build arg array for execpv
          for(int i=0; i<tokens->len;i++) {
            cmd_array[i] = tokens->tokens[i];
          }
          cmd_array[tokens->len] = NULL; // must end with null
          // execute the command
          execvp(tokens->tokens[0],cmd_array);
          
          // error occured
          perror(tokens->tokens[0]);

          _exit(EXIT_FAILURE);
          break;
        
        default:
          break;
        }
        
        id = wait(&status);
        if(id < 0) {
          perror("wait");
        }
        
        else if(status) {
          fprintf(stderr,"command status: %d\n",WEXITSTATUS(status));          
        }
      }

      // Free up memory
      char** it;
      for(it=tokens->tokens; it && *it; ++it) {
        free(*it);
      }
      free(tokens->tokens);
      free(tokens);        

    }             

  }
  return 0;
}

/// Tokenizes the string
TokenStruct* tokenize(char* input) {
  
  char* str = strdup(input);
  int count = 0;
  int capacity = 10;
  TokenStruct* tokStruct = malloc(sizeof(TokenStruct));
  char** tokens = malloc(capacity * sizeof(*tokens));
  
  char* token = strtok(str,DELIMS);

  while(69) {
    // reallocate if too large
    if(count >= capacity) {
      tokens = realloc(tokens,(capacity*=2)*sizeof(*tokens));
    }
    
    tokens[count++] = token ? strdup(token) : token;

    if(!token) break;

    token = strtok(NULL,DELIMS);
  }
  free(str);

  tokStruct->tokens = tokens;
  tokStruct->len = count;
  return tokStruct;
  
}

/// Checks if cmd is an internal command
int is_internal_cmd(char* cmd) {
  
  for(int i=0;i<NUM_CMDS;i++) {
    if(strcmp(cmd,INTERNAL_CMDS[i]) == 0) {
      return 1;
    }
  }
  return 0;
}

/// processes and dispatches internal commands
int process_internal_cmd(TokenStruct* tokens) {
  char* command = tokens->tokens[0];
  int status;
  if(strcmp(command, "verbose") == 0) {
    status = verbose(tokens->len,tokens->tokens);
  }

  else if(strcmp(command, "help") == 0) {
    status = help(tokens->len,tokens->tokens);
  }

  else if(strcmp(command, "history") == 0) {
    status = history(tokens->len,tokens->tokens);
  }

  else if(strcmp(command, "quit") == 0) {
    char** it;
    for(it=tokens->tokens; it && *it; ++it) {
      free(*it);
    }
    free(tokens->tokens);
    free(tokens);
    status = quit(0,NULL);
  }
  return status;
}

/// turn shell verbose on or off
int verbose(int argc, char** argv) {
  if(strcmp(argv[1],"on") == 0) {
    verbose_mode = 1;
  } 

  else if(strcmp(argv[1],"off") == 0) {
    verbose_mode = 0;
  }

  else {
    fprintf(stderr,"Invalid argument for command verbose.");
    return 1;
  }
  return 0;
}

/// prints a list of internal commands
int help(int argc, char** argv) {
  for(int i=0;i<NUM_CMDS;i++) {
    printf("%s\n",INTERNAL_CMDS[i]);
  }
  return 0;
}

/// prints a list of all commands executed so far, and their args
int history(int argc, char** argv) {
  for(int i=0;i<numHist;i++) {
    printf("\t%d: %s",i+1,history_array[i]);
  }
  return 0;
}

/// cleans up memory and quits mish
int quit(int argc, char** argv) {
  exit(EXIT_SUCCESS);
  return 0;
}


