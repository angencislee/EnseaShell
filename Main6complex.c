#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h> 
#include<time.h>

#define CONV_NS_MS 1000000
#define CONV_S_MS 1000
#define MAX_SIZE 256
#define MAX_ARG 100
char linestart[MAX_SIZE] = "enseash % "; 

void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea shell \n Write \'exit' if you want to quit ! \n";
	write(STDOUT_FILENO, "./enseash\n", strlen("./enseash\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	
	}
		
void exitFunction(){
	write(STDOUT_FILENO,"\t Bye bye ...\n\n",strlen("\t Bye bye ...\n\n") );
	exit(EXIT_SUCCESS); 
	}
	
void executeCmde(char **cmde ){
				
		struct timespec time_start, time_stop; // timespec allow to store time information
		
		pid_t pid;
		pid = fork();
		int status;
		clock_gettime(CLOCK_MONOTONIC, &time_start);
		if (pid==-1){
					perror("Error with the fork \n");
				}
		if (pid==0){
					execvp(cmde[0], cmde); //here we use execvp, to use our char tab cmde. Nonetheless execlp we can use multiple arguments.
					exit(EXIT_FAILURE);   // Not executed if there is no error, kills the son if not.
					sprintf(linestart, "enseash %% "); 
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
		argv[0] = strtok(command, " "); //allow us to detect spaces, so we can detect arguments in commands.
		while((argv[i] = strtok(NULL, " "))!=NULL){// also it's possible to detect '-' to find arguments.
			i++; 
		}
		if (strcmp(command,"exit")==0 ||command_size==0){
			exitFunction();
		}
		executeCmde(argv);
			
		
	}
	return 0;
}

