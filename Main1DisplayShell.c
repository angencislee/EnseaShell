#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"Main1DisplayShell.h"

//function to display a welcome message, and a simple prompt
void DisplayShell(){
	char *welcomeMessage = "Welcome to Ensea Shell \n Write \'exit' if you want to quit ! \n";
	char *prompt = "enseash % "; // Display the prompt for the user
	// Use the write function to print on the console
	write(STDOUT_FILENO, "./enseah\n", strlen("./enseah\n"));
	write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
	write(STDOUT_FILENO,prompt,strlen(prompt) );
	}

int main(){
	DisplayShell();
	
	while (1){
		
	
	}
	return 0;
}
