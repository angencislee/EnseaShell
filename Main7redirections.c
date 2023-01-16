#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h> 
#include<time.h>

#define CONV_NS_MS 1000000
#define CONV_S_MS 1000
#define MAX_SIZE 256
#define MAX_ARG 100
char linestart[MAX_SIZE] = "enseash % "; 

void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea  Shell \n Write \'exit' if you want to quit ! \n";
	write(STDOUT_FILENO, "./enseash\n", strlen("./enseash\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	
	}
		
void exitFunction(){
	write(STDOUT_FILENO,"\t Bye bye ...\n\n",strlen("\t Bye bye ...\n\n") );
	exit(EXIT_SUCCESS); 
	}
	
void executeCmde(char **cmde ){
				
		struct timespec time_start, time_stop; 
		char *filename;
		char **tok;
		//int cmpt;
		int chevrons;
		pid_t pid;
		pid = fork();
		int status;
		int in=0;
		int out=0;
		clock_gettime(CLOCK_MONOTONIC, &time_start);
		tok=cmde;
		//checking for the presence of "<" and ">" in the command and storing the next token as the file name.
		while (*tok!=NULL){
			if (strcmp(*tok,"<")==0){
				chevrons=1;
				*tok=NULL;
				in =1; 	
			}
			if (strcmp(*tok,">")==0){
				out=1;
				*tok=NULL;
				chevrons=1;
			}
			tok++;
			if (chevrons==1){
				filename = *tok; 
				break;
			}
			// Variable "chevrons" is set to 1 if "<" or ">" is found in the command
		}
		if (pid==-1){
					perror("Error with the fork \n");
				}
		if (pid==0){
			if (in) { //check if '<' char was found in string inputted by user on the consol
				int fd0 = open(filename, O_RDONLY);
				dup2(fd0, STDIN_FILENO);//duplicate the file descriptor of fd0 to STDIN_FILENO
				close(fd0);
				in = 0;
			}

			if (out) { //check if '>' was found in string inputted by user on the consol
				int fd1 = creat(filename, S_IROTH|S_IRGRP|S_IRUSR);
				dup2(fd1, STDOUT_FILENO);
				close(fd1);
				out = 0;
			}   
			execvp(cmde[0], cmde);
			exit(EXIT_FAILURE);   // Not executed if there is no error, kills the son if not.
			sprintf(linestart, "enseash %% ",WEXITSTATUS(status)); 
			write(STDOUT_FILENO, linestart, strlen(linestart));
		}
		else{
			wait(&status);
			clock_gettime(CLOCK_MONOTONIC, &time_stop);
			unsigned long long int duration_ms = (time_stop.tv_sec-time_start.tv_sec)*CONV_S_MS + (time_stop.tv_nsec-time_start.tv_nsec)/CONV_NS_MS;
			if(WIFEXITED(status)){
				sprintf(linestart, "enseash [exit:%d|%lldms] %% ", WEXITSTATUS(status), duration_ms);
				}
			if(WIFSIGNALED(status)){
				sprintf(linestart, "enseash [sign:%d|%lldms] %% ", WTERMSIG(status), duration_ms);
				} 
			write(STDOUT_FILENO, linestart, strlen(linestart));
			
		}
	}	
	

	
int main(){
	DisplayShell();
	char command[MAX_SIZE]; 
	write(STDOUT_FILENO, linestart, strlen(linestart));
	int command_size;
	int i=1;
	char *argv[MAX_ARG]; 
	
	while (1){
		command_size=read(STDIN_FILENO, command, MAX_SIZE);
		if (command_size!=0){
			command[command_size-1]= '\0';
		}
		argv[0] = strtok(command, " ");//Uses the strtok function to replace the command and store the tokens in the argv array.
		while((argv[i] = strtok(NULL, " "))!=NULL){ // The command is replace by using the space character as a delimiter.
			i++; 
		}
		if (strcmp(command,"exit")==0 ||command_size==0){
			exitFunction();
		}
		executeCmde(argv);
			
		
	}
	return 0;
}

