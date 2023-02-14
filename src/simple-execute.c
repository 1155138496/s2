#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#define WRITE_END 1     // pipe write end
#define READ_END 0      // pipe read end

/*	shell_execute - Function to execute the command with specified command content and command arguments. 
*	@args	String array with command and its arguments. 
*	@argc	Total number of strings including the command, its arguments and the NULL in the end
*/

	 


int shell_execute(char ** args, int argc){


  int child_pid, wait_return, status;

	/* Execute built-in commands */
	/* exit */
	if(strcmp(args[0], "exit") == 0 ){
		return -1; 
	}
	/* TODO: cd */
	if(strcmp(args[0], "cd")== 0){
		if(chdir(args[1])<0){
			printf("cd: %s: %s ", args[1],strerror(errno));
		}
		return 0;
	}
	argc -= 1;
    for (int f = 0; f < argc; f++) {
        if (strchr(args[f], '|') == NULL){
		
  /* Non-built-in commands. These commands should be executed in a child process so the parent process can continue to invoke other commands */	
	/* One command without pipe */
	if((child_pid = fork()) < 0){
		printf("fork() error \n");
	}
	else if(child_pid == 0 ){
		/* TODO: execute the command and check if the command is correctly executed */
		if(execvp(args[0],args)<0){
			printf("execute error\n");
			exit(-1);
		}
	}else{
		if ((wait_return=wait(&status))<0)
		printf("wait error\n");
		
	}
        }else {
	
	/* TODOs: one pipe and two pipes */

    // /* One pipe */
	// int fd[2]; int pid;
	// char * l[argc], r[argc];
	// int lcnt = 0, rcnt = 0;

	// // Check if there's a pipe symbol and split the commands
	// for (int i = 0; i < argc; i++) {
	// 	if (strcmp(args[i], "|") == 0) {
	// 		for (int j = 0; j < i; j++) {
	// 			l[j] = args[j];
	// 		}
	// 		l[i] = NULL;
	// 		for (int j = i + 1; j < argc; j++) {
	// 			r[rcnt] = args[j];
	// 			rcnt++;
	// 		}
	// 		r[rcnt] = NULL;
	// 		break;
	// 	} else {
	// 		l[lcnt] = args[i];
	// 		lcnt++;
	// 	}
	// }

	// if (rcnt > 0) {
	// 	if (pipe(fd) < 0) {
	// 		printf("pipe() error\n");
	// 		return -1;
	// 	}
	// 	if ((pid = fork()) < 0) {
	// 		printf("fork() error\n");
	// 	} else if (pid == 0) {
	// 		// Child process
	// 		close(fd[0]);
	// 		dup2(fd[1], STDOUT_FILENO);
	// 		close(fd[1]);
	// 		if (execvp(r[0], r) < 0) {
	// 			printf("execute error\n");
	// 			exit(-1);
	// 		}
	// 	} else {
	// 		// Parent process 
	// 		close(fd[1]);
	// 		dup2(fd[0], STDIN_FILENO);
	// 		close(fd[0]);
    //         if(execvp(l[0],l)<0){
    //             printf("execute error\n");
    //             exit(-1);
    //         }
    //         }
    //         }

            /* Two pipes */
int fd1[2], fd2[2];
int pid1, pid2, c1cnt = 0, c2cnt = 0, c3cnt = 0;
char * c1[argc], c2[argc], c3[argc];

// Check if there are two pipe symbols and split the commands
for (int i = 0; i < argc; i++) {
    if (strcmp(args[i], "|") == 0 ) {
        for (int j = 0; j < i; j++) {
            c1[c1cnt] = args[j];
            c1cnt++;
        }
        c1[c1cnt] = NULL;

        for(int k=i+1; k<argc;k++){
            if(strcmp(args[k],"|")==0){
                for(int s=i+1;s<k;s++){
                    c2[c2cnt]=args[s];
                    c2cnt++;
                }
                c2[c2cnt]=NULL;
            }
        
            for (int j = k+1; j < argc; j++) {
            c3[c3cnt] = args[j];
            c3cnt++;
        }
        c3[c3cnt] = NULL;
        }

        break;
    }
}

if (c3cnt > 0) {
    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("pipe() error\n");
        return -1;
    }
    if ((pid1 = fork()) < 0) {
        printf("fork() error\n");
    } else if (pid1 == 0) {
        // Child process 
        close(fd1[0]);
        close(fd2[1]);
        dup2(fd1[1], STDOUT_FILENO);
        dup2(fd2[0], STDIN_FILENO);
        close(fd1[1]);
        close(fd2[0]);
        if (execvp(c2[0], c2) < 0) {
            printf("execute error\n");
            exit(-1);
        }
    } else {
        if ((pid2 = fork()) < 0) {
            printf("fork() error\n");
        } else if (pid2 == 0) {
            // Child process 
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd1[0], STDIN_FILENO);
            close(fd1[0]);
            dup2(fd2[1], STDOUT_FILENO);
            close(fd2[1]);
            if (execvp(c3[0], c3) < 0) {
                printf("execute error\n");
                exit(-1);
            }
        } else {
            // Parent process 
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd2[1], STDOUT_FILENO);
            close(fd2[1]);
            if (execvp(c1[0], c1) < 0) {
                printf("execute error\n");
                exit(-1);
                }
            }
            }
        }
    }
}







 /* wait for child process to terminate */
	while((wait_return = wait(&status)) > 0);
			
 return 0;

}