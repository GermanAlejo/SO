#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char *argv[] ){
    char buffer[512];
    printf( "El directorio actual es: %s\n",getcwd( buffer, 512) );
	printf("Buffer %s\n",buffer);
    chdir( ".." );
printf( "El directorio actual es: %s\n",getcwd( buffer, 512) );
    mkdir( "./directorio1", 0755 );
    mkdir( "./directorio2", 0755 );
    rmdir( "./directorio1" );
return 0;
}
