#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h> 
#include<time.h>

#define CONV_NS_MS 1000000
char linestart[256] = "enseash % "; 

void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea Shell \n Write \'exit' if you want to quit ! \n";
	write(STDOUT_FILENO, "./enseash\n", strlen("./enseash\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	
	}
		

void executeCmde( char *cmde ){
		
		pid_t pid;
		pid = fork();
		struct timespec time_start, time_stop; 
		int status;
		clock_gettime(CLOCK_REALTIME, &time_start); //allow us to obtain the time data with a specified ID
		if (pid==-1){
					perror("Error with the fork \n");
				}
		if (pid==0){
					execlp(cmde, cmde, NULL);
					exit(EXIT_FAILURE);
					sprintf(linestart, "enseash % ",WEXITSTATUS(status)); 
					write(STDOUT_FILENO, linestart, strlen(linestart));
				}
		else{
			wait(&status);
			clock_gettime(CLOCK_REALTIME, &time_stop);
			unsigned long long int duration = time_stop.tv_nsec-time_start.tv_nsec;
			//duration is a difference which gives the time between the start and the end.
			if(WIFEXITED(status)){
				sprintf(linestart, "enseash [exit:%d|%lldms] %% ", WEXITSTATUS(status), duration/CONV_NS_MS);
				}
			if(WIFSIGNALED(status)){
				sprintf(linestart, "enseash [sign:%d|%lldms]% % ", WTERMSIG(status), duration/CONV_NS_MS);
				} 
			write(STDOUT_FILENO, linestart, strlen(linestart));
			
		}
	}


	
void exitFunction(){
	write(STDOUT_FILENO,"\t Bye bye ...\n\n",strlen("\t Bye bye ...\n\n") );
	exit(EXIT_SUCCESS); 
	}



int main(){
	DisplayShell();
	char command[256]; 
	int number;
	write(STDOUT_FILENO, linestart, strlen(linestart));
	
	while (1){
		
		number =read(STDIN_FILENO,command,256);
		if (number!=0){
			command[number-1]= '\0';
		}
		if (strcmp(command,"exit")==0 ||number==0){
			exitFunction();
		}
		executeCmde(command);	
		
	}
	return 0;
}
