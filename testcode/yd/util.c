#include "util.h"

#define READ_BUF_SIZE 1024

long* find_pid_by_name( char* pidName) 
{ 
            DIR *dir; 
            struct dirent *next; 
            long* pidList=NULL; 
            int i=0; 


            dir = opendir("/proc"); 
            if (!dir) 
            { 
                    //perror_msg_and_die("Cannot open /proc"); 
                    fprintf(stderr, "Cannot open /proc\n"); 
                    return pidList; 
            } 

            while ((next = readdir(dir)) != NULL) 
            { 
                    FILE *status; 
                    char filename[READ_BUF_SIZE]; 
                    char buffer[READ_BUF_SIZE]; 
                    char name[READ_BUF_SIZE]; 

                    /* Must skip ".." since that is outside /proc */ 
                    if (strcmp(next->d_name, "..") == 0) 
                            continue; 

                    /* If it isn't a number, we don't want it */ 
                    if (!isdigit(*next->d_name)) 
                            continue; 
                    sprintf(filename, "/proc/%s/status", next->d_name); 

                    if (! (status = fopen(filename, "r")) ) 
                    { 
                            continue; 
                    } 

                    //Read first line in /proc/?pid?/status 

                    if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL) 
                    { 
                            fclose(status); 
                            continue; 
                    } 

                    fclose(status); 

                    // Buffer should contain a string like "Name: binary_name"a 

                    sscanf(buffer, "%*s %s", name);
                    if ( name != NULL && name[0] != '\0')
                    {
                            if (strcmp(name, pidName) == 0)
                            {
                                    pidList=realloc( pidList, sizeof(long) * (i+2));
                                    pidList[i++]=strtol(next->d_name, NULL, 0);
                            }

                    }

            }

            if (pidList)
            {
                    pidList[i]=0;
            }
            return pidList;
}


 
int get_file_time(char *sFileName, char* sFileTime)
{
        struct stat st;
        if(stat(sFileName, &st) == -1)
        {
                  printf("get %s file info failed\n", sFileName);
                  return -1;
        }
        char sTime[64] = {0};
        strftime(sTime, 64, "%Y-%m-%d %H:%M", localtime(&st.st_mtime));
        memcpy(sFileTime, sTime, strlen(sTime));
        return 0;
                
}

int timevaltochartime(time_t tv_sec, char * out_time)
{
        char   timef[64] = {0};
        time_t   t;
        struct   tm   *today;
        t = tv_sec;
        today   =   localtime(&t);
        strftime(timef, 64, "%Y-%m-%d   %H:%M:%S ",today);
        memcpy(out_time, timef, strlen(timef));

}



void str_split(const char* ori, char* des, char* ori_separator, char* des_separator) //ori can not change  
{  
    char *token = NULL;  
  
    char *pStrTemp = (char*)malloc(strlen(ori) + 1);  
    memcpy(pStrTemp, ori, strlen(ori) + 1);  
    char *p = pStrTemp;  
      
    while((token = strsep(&p, ori_separator)) != NULL)  
    {  
    	if(token != NULL)
	{  
        	strcpy(des, token);
		des = des+strlen(des); 
		strcpy(des, des_separator);
		des = des+1; 
	}
    	else
	{  
        	strcpy(des, "");
	}  
         
    }  
  
  
    free(pStrTemp);  
           
}

 
void inet_ntoa_r(unsigned int ip, char ipstr[MAX_IP_LEN])
{
        unsigned char *p = (unsigned char *)&ip;
        snprintf(ipstr, MAX_IP_LEN, "%d.%d.%d.%d", p[3], p[2], p[1], p[0]);
}



/*
 - split - divide a string into fields, like awk split()
 = int split(char *string, char *fields[], int nfields, char *sep);
 */
//char *string;
//char *fields[];                       /* list is not NULL-terminated */
//int nfields;                  /* number of entries available in fields[] */
//char *sep;                    /* "" white, "c" single char, "ab" [ab]+ */
//return value                          /* number of fields, including overflow */
int split(char *string, char* fields[], int nfields,char* sep)
{
        register char *p = string;
        register char c;                        /* latest character */
        register char sepc = sep[0];
        register char sepc2;
        register int fn;
        register char **fp = fields;
        register char *sepp;
        register int trimtrail;

        /* white space */
        if (sepc == '\0') {
                while ((c = *p++) == ' ' || c == '\t')
                        continue;
                p--;
                trimtrail = 1;
                sep = (char*) " \t";    /* note, code below knows this is 2 long */
                sepc = ' ';
        } else
                trimtrail = 0;
        sepc2 = sep[1];         /* now we can safely pick this up */

        /* catch empties */
        if (*p == '\0')
                return(0);

        /* single separator */
        if (sepc2 == '\0') {
                fn = nfields;
                for (;;) {
                        *fp++ = p;
                        fn--;
                        if (fn == 0)
                                break;
                        while ((c = *p++) != sepc)
                                if (c == '\0')
                                        return(nfields - fn);
                        *(p-1) = '\0';
                }
                /* we have overflowed the fields vector -- just count them */
                fn = nfields;
                for (;;) {
                        while ((c = *p++) != sepc)
                                if (c == '\0')
                                        return(fn);
                        fn++;
                }
                /* not reached */
        }

        /* two separators */
        if (sep[2] == '\0') {
                fn = nfields;
                for (;;) {
                        *fp++ = p;
                        fn--;
                        while ((c = *p++) != sepc && c != sepc2)
                                if (c == '\0') {
                                        if (trimtrail && **(fp-1) == '\0')
                                                fn++;
                                        return(nfields - fn);
                                }
                        if (fn == 0)
                                break;
                        *(p-1) = '\0';
                        while ((c = *p++) == sepc || c == sepc2)
                                continue;
                        p--;
                }
                /* we have overflowed the fields vector -- just count them */
                fn = nfields;
                while (c != '\0') {
                        while ((c = *p++) == sepc || c == sepc2)
                                continue;
                        p--;
                        fn++;
                        while ((c = *p++) != '\0' && c != sepc && c != sepc2)
                                continue;
                }
                /* might have to trim trailing white space */
                if (trimtrail) {
                        p--;
                        while ((c = *--p) == sepc || c == sepc2)
                                continue;
                        p++;
                        if (*p != '\0') {
                                if (fn == nfields+1)
                                        *p = '\0';
                                fn--;
                        }
                }
                return(fn);
        }

        /* n separators */
        fn = 0;
        for (;;) {
                if (fn < nfields)
                        *fp++ = p;
                fn++;
                for (;;) {
                        c = *p++;
                        if (c == '\0')
                                return(fn);
                        sepp = sep;
                        while ((sepc = *sepp++) != '\0' && sepc != c)
                                continue;
                        if (sepc != '\0')       /* it was a separator */
                                break;
                }
                if (fn < nfields)
                        *(p-1) = '\0';
                for (;;) {
                        c = *p++;
                        sepp = sep;
                        while ((sepc = *sepp++) != '\0' && sepc != c)
                                continue;
                        if (sepc == '\0')       /* it wasn't a separator */
                                break;
                }
                p--;
        }
        /* not reached */
}

void strtohex(u_char *dest, u_char *src, int nLen)
{
	char h1,h2;
	u_char s1,s2;
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = src[2*i];
		h2 = src[2*i+1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
			s2 -= 7;

		dest[i] = s1*16 + s2;
	}
}
