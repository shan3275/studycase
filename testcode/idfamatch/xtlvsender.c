#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "cpcre.h"


//read a line from a command line.
static char *getLine( FILE *fp, char *__dst ) 
{
    register int c;
    register char *cs;

    cs = __dst;
    while ( ( c = getc( fp ) ) != EOF ) {
        if ( c == '\n' ) break;
        *cs++ = c; 
    }
    *cs = '\0';

    return ( c == EOF && cs == __dst ) ? NULL : __dst;
}

static double getTime()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000 + ((double)tv.tv_usec)/1000);
}


static void simple_idfa(char* idfa, char* simidfa)
{
	char* sim_begin = strstr(idfa, "=") + 1;
	char* sim_end = idfa + strlen(idfa);
	memcpy( simidfa, sim_begin , sim_end - sim_begin);
}

static void printHex( void* val, size_t vlen )
{
	char* cval = val;
	for(int i = 0 ; i < vlen; i++)
	{
		printf("%02x ", *(cval + i));
	}
	printf("\n");
}

void tlv_send( char *ipfile)
{
    FILE *pFile = fopen(ipfile, "r");
    if (!pFile) 
        return ;
    char CmdLine[2048] = {0};
    char* p = NULL; 
    
    while(fscanf(pFile,"%[^\n]",CmdLine)!=EOF)
    {
	CmdLine[ strlen(CmdLine) - 1 ] = '\0';	
	p = CmdLine;

	char idfa[64] = {0};
	int ret = cpcre_indo_idfa_match( CmdLine, idfa) ;	
	if(ret == 0)
	{
		printf("%s\n", idfa);
	}
	
	memset(CmdLine, '\0', 2048);
	fgetc(pFile);
    }
    fclose(pFile);


}


int main( int argc, char **argv )
{

     cpcre_idfa_init() ;
     tlv_send( "idfa_url_all.txt");

     cpcre_idfa_close();
     return 0;
}
