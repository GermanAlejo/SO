#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "practica9"

main()
{
    char s[300];
    int num, fd;
    
    /* don't forget to error check this stuff!! */
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    
    printf("waiting for readers...\n");
    fd = open(FIFO_NAME, O_WRONLY);
    printf("got a reader--type some stuff\n");
    
    //while (gets(s), !feof(stdin)) {
	while ( fgets(s, sizeof(s), stdin), !feof(stdin)){
	if ((num = write(fd, s, strlen(s))) == -1)
	    perror("write");
	else
	    printf("speak: wrote %d bytes\n", num);
    }
}
