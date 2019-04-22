/***** KEYBOARD HIT PROGRAM *****/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>

int filedes[2];

void read_char()
{
	char c;
	 char s[300];
	int num;
	printf("Entering routine to read character.........\n Type a character: \n");
	c = getchar();
	//c = getc(stdin);
	while (c != '0'){
		if (c != '\n'){
			if ((num = write(filedes[1], &c, 1)) == -1){
           			 perror("write");
				 exit(1);
			}
	        	else
        		         printf("Child: wrote character: %c\n",c);
		
			printf("Child: Type a character: \n");
		}		
		c = getchar();
		
	}
	write(filedes[1], &c, 1);
}

void check_hit()
{
	char c;
	char s[300];
	int num;
	printf("Entering routine to check hit.........\n");
	do {
		//printf("Father: waiting\n");
		if ((num = read(filedes[0], &c, 1)) == -1)
	            perror("read");
	        else {	   
			if (c != '\n')	    
	            		printf("Father: bytes %d read  \"%c\"\n",num,  c);
	        }
    } while (c != '0');
}
		
int main()
{
	int i;
	pthread_t tid1, tid2;
	pipe(filedes);
	int pid	= fork();
	if (pid == -1)
		exit(1);
	if (pid == 0){
		//hijo
		read_char();
	}
	else{
		check_hit();
	}
	exit(0);
}
