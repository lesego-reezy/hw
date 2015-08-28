#include "process.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <termios.h>

/**
 * Executes the process p.
 * If the shell is in interactive mode and the process is a foreground process,
 * then p should take control of the terminal.
 */
void launch_process(process *p)
{
  /** YOUR CODE HERE */
    pid_t pid;

    if(shell_is_interactive){
        pid = getpid();
        printf("Launch process %d\n", pid);//for debugging

        setpgid(pid,pid);
        if(!p->background){
            tcsetpgrp(shell_terminal,pid);
            tcsetattr(shell_terminal,TCSADRAIN, &p->tmodes);
        }
        //set all signals to default
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);

        if(p->stdin!=STDOUT_FILENO){
            dup2(p->stdout,STDOUT_FILENO);
            close(p->stdout);
        }
        if(p->stderr!=STDIN_FILENO){
            dup2(p->stderr,STDERR_FILENO);
            close(p->stderr);
        }
    }

}

/* Put a process in the foreground. This function assumes that the shell
 * is in interactive mode. If the cont argument is true, send the process
 * group a SIGCONT signal to wake it up.
 */
void
put_process_in_foreground (process *p, int cont)
{
  /** YOUR CODE HERE */

}

/* Put a process in the background. If the cont argument is true, send
 * the process group a SIGCONT signal to wake it up. */
void
put_process_in_background (process *p, int cont)
{
  /** YOUR CODE HERE */
}
