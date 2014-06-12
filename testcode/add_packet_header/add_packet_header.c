#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "add_packet_header.h"
#include "md5.h"

int do_name, do_gf_name;
char *l_opt_arg;

static const char *shortopts = "l:v:t:i:o:?";
struct option longopts[] = {
{"logo",     required_argument, NULL, 'l'},
{"vsersion", required_argument, NULL, 'v'},
{"time",     required_argument, NULL, 't'},
{"input",    required_argument, NULL, 'i'},
{"output",   required_argument, NULL, 'o'},
{"?",        no_argument,       NULL, '?'},
{0,          0,                 0,     0 },
};
void
cmd_usage(char* program)
{
    printf("%s [-l <logo>] [-v <version>] [-t <MMDDhhmmyear.ss>] -f <FILE> \n", program);
    printf("    -l         logo, default:semptian, length <= 8 bytes.\n");
    printf("    -v         version, defalt:1.1.0.\n");
    printf("    -t         time, default current system time.\n");
    printf("    -i         input file.\n");
    printf("    -o         output file.\n");
}


/* get file stat */
struct stat get_stat(char *pathname)
{
     struct stat buf;
     int fd;

        /* open the file */
    if ((fd = open(pathname, O_RDONLY)) < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* get file stats*/
    if ((fstat(fd, &buf)) < 0) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
   // printf("    SIZE: %ld\n", buf.st_size);

     /* close the file */
    if (close(fd) < 0) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return buf;

}

/* one input parameter to generate upgrade file */
int combine_file_with_packet(char *packet, char *pathname, char* outpathname)
{
    int nread = 0;
    char buff[READ_BUFF]={0};
    FILE *fp_read,*fp_write;

    if((fp_read=fopen(pathname,"rb"))==NULL)
    {
        printf("Cann't open %s !",pathname);
        return 1;
    }
    if((fp_write=fopen(outpathname,"wb"))==NULL)
    {
        printf("Cann't open %s !",outpathname);
        return 1;
    }
    fwrite((char *)packet, sizeof(char), 64, fp_write);

    while((nread=fread(buff,sizeof(char),READ_BUFF,fp_read))>0)
    {
      fwrite(buff,sizeof(char),nread,fp_write);
    }

    fclose(fp_read);
    fclose(fp_write);
    return 0;
}

int main (int argc, char *argv[])
{
    int c, rv;

    packet_header header;

    uchar *logo;   /* flag */
    uchar *version;
    uchar major = 1, revision = 1;
    ushort build = 0;
    ushort year;
    uchar  month, day;
    uchar  hour, minute, second;
    uchar md5[32];

    uchar flg=0;   /* flag ,bit 0 for logo; bit 1 for version; bit 2 for time */
    time_t timer;
    struct tm *tp;

    char *pathname;
    char *output_pathname;
    struct stat file_stat;

    if (argc < 2){
        cmd_usage(argv[0]);
        }

    memset(&header, 0, sizeof(packet_header));
    memset(md5, 0, 32);

    while ((c = getopt_long (argc, argv, shortopts, longopts, NULL)) != -1)
        {
        switch (c)
            {
            case 'l':
                flg |= 1;
                //strncpy((char *)logo, optarg, sizeof(logo));
                logo = optarg;
                printf("logo:%s\n", logo);
                break;

            case 'v':
                flg |= 2;
                version = optarg;
                break;

            case 't':
                flg |= 4;
                rv = sscanf(optarg, "%2u%2u%2u%2u%4u.%2u", &month, &day, &hour, &minute, &year, &second);
                if (rv != 6)
                    {
                    printf("time format error!\n");
                    return 1;
                    }
                break;

            case 'i':
                pathname = optarg;
                break;

            case 'o':
                output_pathname = optarg;
                break;
            }

        }

    /* write flag to packet header */
    if (flg & 0x1)
        {
        strncpy((char *)header.packet_flag, logo, sizeof(header.packet_flag));
        printf("packet_flg:%s\n", header.packet_flag);
        printf("logo:%s\n", logo);
        } else
        {
        strncpy((char *)header.packet_flag, "semptian" , sizeof(header.packet_flag));
        }

        /* write time to packet header */
    if (flg & 0x4)
        {
            header.time.year = year;
            header.time.month = month;
            header.time.day   = day;
            header.time.hour  = hour;
            header.time.minute = minute;
            header.time.second = second;
        } else
        {
            timer=time(NULL);
            tp=localtime(&timer);

            // write time
            header.time.year  = (uint)(tp->tm_year+1900);
            header.time.month =  tp->tm_mon+1 ;
            header.time.day   =  tp->tm_mday ;
            header.time.hour   = tp->tm_hour ;
            header.time.minute = tp->tm_min;
            header.time.second = tp->tm_sec ;
        }


        if (flg &0x2)
        {
            rv = sscanf("1:1:34", "%d:%d:%d", &major, &revision, &build);
            printf("version %s major:%d, revison:%d, build:%d\n",  version,major,  revision,  build);
            if (rv != 3)
            {
                printf("version format");
                return 1;
            }
        }
        /* write version to packet header */
        header.version.major    = major;
        header.version.revision = revision;
        header.version.build    = build;

        /* write md5 value to packet header */
        strncpy((char *)header.md5, MDFile(pathname), 32);

        /* write packet header length to packet header s*/
        header.packet_len = 64;

        /* write file length to packet header */
        file_stat = get_stat(pathname);
        header.file_len = file_stat.st_size;

        /* generate new file */
        combine_file_with_packet((char *)&header, pathname, output_pathname);

    return 0;
}
