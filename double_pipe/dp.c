/*--------------------------------------------------------------------

File: dp.c
Author: algorhtym


Description:  Double pipe program.  To pipe the output from the standard
          output to the standard input of two other processes.
          Usage:  dp  <cmd1> : <cmd2> : <cmd3>
          Output from process created with cmd1 is piped to
          processes created with cmd2 and cmd3

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
/* prototypes */
int doublePipe(char **,char **,char **);

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{

   int i,j;         /*indexes into arrays */
   char *cmd1[10];  /*array for arguments of first command */
   char *cmd2[10];  /*array for arguments of second command */
   char *cmd3[10];  /*array for arguments of third command */


   
   if(argc == 1)
   {
     printf("Usage: dp <cmd1 arg...> : <cmd2 arg...> : <cmd3 arg....>\n");
     exit(1);
   }

   /* get the first command */
   for(i=1,j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found first command */
      cmd1[j]=argv[i];
   }
   cmd1[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only one command found\n");
      exit(1);
   }
   else i++;

   /* get the second command */
   for(j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found second command */
      cmd2[j]=argv[i];
   }
   cmd2[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only two commands found\n");
      exit(1);
   }
   else i++;

   /* get the third command */
   for(j=0 ; i<argc ; i++,j++) cmd3[j]=argv[i];
   cmd3[j]=NULL;
   if(j==0) /* no command after last : */
   {
      printf("Bad command syntax - missing third command\n");
      exit(1);
   }

   exit(doublePipe(cmd1,cmd2,cmd3));
}

/*--------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

int doublePipe(char **cmd1, char **cmd2, char **cmd3)
{
  /*PIDs for the three processes to be created*/
  pid_t prog1, prog2, prog3, prog1dup;

  /*defining the read buffers for program 2 and program 3*/
  char readbuffer2[40];
  char readbuffer3[40];
  char writebuffer[40];

  int num_bytes2, num_bytes3;

  /*file decriptors for the two pipes to be created*/
  int fd2[2], fd3[2];

  char *program1, *program2, *program3;
  program1 = cmd1[0];
  program2 = cmd2[0];
  program3 = cmd3[0];

  /*creating the two pipelines*/
  pipe(fd2);
  pipe(fd3);
  
  /*Running the first program*/
  prog1 = fork();
  if (prog1 == -1) {
    perror("First fork failed!");
    exit(1);
  }

  else if (prog1 == 0) {
    

    /*Duplicating the file decriptors' write end onto the standard output of process 1*/

   
    dup2(1, fd2[1]);
    //fd3[1] = dup(fd2[1]);
    //dup2(fd3[1], fd2[1]);

    /*Closing the read end of the pipes from prog1*/
    
    close(fd2[0]);
    close(fd2[1]);
    close(fd3[0]);
    close(fd3[1]);


    /*executing the first program*/
    execvp(program1, cmd1);
  } else {
    
    /*Running program 2*/
    prog2 = fork();

    if (prog2 == -1) {
      perror("Second fork failed!");
      exit(1);
    }
  
    else if (prog2 == 0) {
      
      /*Duplicating the file descriptor's read end to the std input of process 2*/
      dup2(0, fd2[0]);
      //dup2(fd2[1], 1);
      //dup2(fd3[1], 1);

      /*Closing both ends of the pipes*/
      close(fd2[0]);
      close(fd2[1]);
      close(fd3[0]);
      close(fd3[1]);
      

      /*executing the second program*/
      execvp(program2, cmd2);

    }
 
  }

  /*Running the first program*/
  prog1dup = fork();
  if (prog1dup == -1) {
    perror("First fork failed!");
    exit(1);
  }

  else if (prog1dup == 0) {
    

    /*Duplicating the file decriptors' write end onto the standard output of process 1*/

   
    dup2(1, fd3[1]);
    //fd3[1] = dup(fd2[1]);
    //dup2(fd3[1], fd2[1]);

    /*Closing the read end of the pipes from prog1*/
    
    close(fd2[0]);
    close(fd2[1]);
    close(fd3[0]);
    close(fd3[1]);


    /*executing the first program*/
    execvp(program1, cmd1);
  } else {
  
    /*Running program 3 */
    prog3 = fork();
    
    if (prog3 == -1) {
      perror("Third fork failed!");
      exit(1);
    }
      
    else if (prog3 == 0) {
      
	
      /*Duplicating the file descriptor's read end to the std input of process 2*/
      dup2(0, fd3[0]);
      //dup2(fd3[1], 1);

      /*Closing both ends of the pipes*/
      close(fd2[0]);
      close(fd2[1]);
      close(fd3[0]);
      close(fd3[1]);
      
      /*executing the second program*/
      execvp(program3, cmd3);
    }
      
  }
  
  /*print what is read from pipe 2 on shell*/
  // num_bytes2 = read(fd2[0], readbuffer2, sizeof(readbuffer2));
  //printf("%s", readbuffer2);
  
  /*print what is read from pipe 3 on shell*/
  //num_bytes3 = read(fd3[0], readbuffer3, sizeof(readbuffer3));
  //printf("%s", readbuffer3);
  
 

  printf("\nSleeping for 5 secs.\n");
  sleep(2);
  
  /*closing all ends of the pipe in the parent process*/
  close(fd2[0]);
  close(fd2[1]);
  close(fd3[0]);
  close(fd3[1]);

  
  // Kill program1, program2 and program3
  kill(prog1, SIGTERM);
  kill(prog2, SIGTERM);
  kill(prog3, SIGTERM);
  
  return 0;
}
