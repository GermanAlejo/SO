#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main( int argc, char *argv[] )
{
   int fd;
   if( (fd = open( argv[1], O_RDWR )) == -1 )
    {
		perror( "open" );
		exit( -1 );
     }
    printf( "El archivo abierto tiene el descriptor %d.\n", fd );
    close( fd );
return 0;
}

