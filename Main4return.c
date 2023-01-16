#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h> 

char linestart[256] = "enseash % "; 

void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea Shell \n Write \'exit' if you want to quit ! \n";
	write(STDOUT_FILENO, "./enseash\n", strlen("./enseash\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	
	}
		



/*We want now the return code
*/	
void executeCmde( char *cmde ){
		
		pid_t pid;
		pid = fork();
		int status;
		if (pid==-1){  //verify if there is an error when resetting the command then send a error message
					perror("Error with the fork \n");
				}
		if (pid==0){
					execlp(cmde, cmde, NULL);
					exit(EXIT_FAILURE);
					printf(linestart, "enseash % "); 
					write(STDOUT_FILENO, linestart, strlen(linestart));
				}
		else{
			wait(&status);
			if(WIFEXITED(status)){
				sprintf(linestart, "enseash [exit:0] % % ", WEXITSTATUS(status));
				//WIFEXITED return a non null value if the son process exists
				}
			if(WIFSIGNALED(status)){
				sprintf(linestart, "enseash [sig:9] % % ", WTERMSIG(status));
				//WIFSIGNALED returns 9 if the son process has been killed
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
