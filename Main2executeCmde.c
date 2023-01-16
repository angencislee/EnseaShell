#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h> 



void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea Shell \n Write \'exit' if you want to quit ! \n";
	write(STDOUT_FILENO, "./enseash\n", strlen("./enseash\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	}
		

/*
Function to execute the command entered by the user in the consol
The input is the command entered by the user
*/	
void executeCmde( char * cmde ){
		// This code creates a child process using the fork() function
		pid_t pid;
		int status;
		// Use the fork() function to create a child process
		pid = fork();
		
		 
		if (pid==-1){
				perror("Error with the fork\n"); // error message
				}
		if (pid==0){
				execlp(cmde, cmde, NULL);
				// execlp can execute a file and thanks to the fork, it can execute the command
			}
		else{
			wait(&status);
		}
		
		
		
	}

	
		
	

int main(){
	DisplayShell();
	char command[256]; // to store the command input by the user
char command[256];
	int number;
	
	while (1){
		write(STDOUT_FILENO, "enseash % ", strlen("enseash % "));
		number =read(STDIN_FILENO,command,256);
		
		command[number-1]= '\0';
		executeCmde(command); 
		
		
	
	}
	return 0;
}


