#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned long long u64;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

struct pcap_file_header {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t thiszone; /* gmt to local correction */
    uint32_t sigfigs;    /* accuracy of timestamps */
    uint32_t snaplen;    /* max length saved portion of each pkt */
    uint32_t linktype;   /* data link type (LINKTYPE_*) */
};

struct pcap_pkthdr {
    uint32_t second;  /* time stamp */
    uint32_t micresec; /* time stamp */
    uint32_t caplen; /* length of portion present */
    uint32_t len;    /* length this packet (off wire) */
};


struct pcap_file_struct{
    int infd; /* input file descriptor */
    int outfd; /* output file descriptor*/
};

#define DUMP_LINE_SIZE      128
static inline void pcap_dump_buff(int len, uchar *buff)
{
    int i;
    char *p, line[DUMP_LINE_SIZE];

    p = line;
    printf("BUFF DUMP %d bytes:\n", len);
    for (i = 0; i < len; i++)
    {
        if ((i % 16) == 0)
        {
            memset(line, 0, DUMP_LINE_SIZE);
            p = line;
            p += sprintf(p, "%.4d:", i);
        }

        if ((i % 16) == 8)
        {
            p += sprintf(p, " -");
        }
        p += sprintf(p, " %.2x", buff[i]);

        if ((i % 16) == 15)
        {
            line[DUMP_LINE_SIZE - 1] = 0;
            printf("%s\n", line);
        }
    }
    if ((i % 16) != 0)
    {
        line[DUMP_LINE_SIZE - 1] = 0;
        printf("%s\n", line);
    }
    printf("\n");
}

void pcap_header_usage(char* program)
{
    printf("Input like this:%s input_file out_file length\n", program);
    printf("input_file : input file name.\n");
    printf("out_file   : output file name.\n");
    printf("length     : packet num.\n");
}

int pcap_file_init(char* in_file, char* out_file, struct pcap_file_struct *file_des)
{
    int in_fd, out_fd; /* file description */
    uint32_t numbytes;
    struct pcap_file_header pcap_header;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;

    memset(&pcap_header, 0, sizeof(struct pcap_file_header));
    /* open file */
    if ((in_fd = open(in_file, O_RDONLY, 0644)) < 0) {
        perror("open");
        return EXIT_FAILURE;
    }else{
        printf("opened %s\n",in_file);
        printf("descriptor is %d\n", in_fd);
    }

    /* read pacap header */
    if (read(in_fd, &pcap_header, sizeof(struct pcap_file_header))
        != sizeof(struct pcap_file_header))
    {
        printf("read pcap header failed!\n");
        close(in_fd);
    }else {
        printf("pcap_header.magic = 0x%x\n", pcap_header.magic);
        printf("pcap_header.version_major = 0x%x\n", pcap_header.version_major);
        printf("pcap_header.version_minor = 0x%x\n", pcap_header.version_minor);
        printf("pcap_header.thiszone = 0x%x\n", pcap_header.thiszone);
        printf("pcap_header.sigfigs = 0x%x\n", pcap_header.sigfigs);
        printf("pcap_header.snaplen = 0x%x\n", pcap_header.snaplen);
        printf("pcap_header.linktype = 0x%x\n", pcap_header.linktype);
    }

    /* open outfile */
    if ((out_fd = open(out_file, flags, 0644)) < 0) {
        perror("open");
        close(in_fd); /* have to close both of these now */
        close(out_fd);
        return EXIT_FAILURE;
    }

    /*write pcap header */
    numbytes = write(out_fd, &pcap_header, sizeof(struct pcap_file_header));
    if (numbytes != sizeof(struct pcap_file_header))
    {
        printf("write pcap header failed!\n");
        close(in_fd);
        close(out_fd);
        return EXIT_FAILURE;
    }

    file_des->infd = in_fd;
    file_des->outfd = out_fd;
    return 0;
}

int pcap_file_add_packet(uchar *buff, struct pcap_file_struct *file_des)
{
    int in_fd, out_fd; /* file description */
    uint32_t numbytes;
    struct pcap_pkthdr pkt_header;
    uint32_t packet_len;

    in_fd = file_des->infd;
    out_fd = file_des->outfd;

    memset(&pkt_header,  0, sizeof(struct pcap_pkthdr));

    /* read packet header*/
    if (read(in_fd, &pkt_header, sizeof(struct pcap_pkthdr))
        != sizeof(struct pcap_pkthdr))
    {
        printf("read pcap packet header failed!\n");
    }else {
        printf("pkt_header.second = 0x%x\n", pkt_header.second);
        printf("pkt_header.micresec = 0x%x\n", pkt_header.micresec);
        printf("pkt_header.caplen = 0x%x\n", pkt_header.caplen);
        printf("pkt_header.len = 0x%x\n", pkt_header.len);
    }

    packet_len = pkt_header.caplen;
    printf("%d pkt_header.caplen = 0x%x\n", __LINE__, pkt_header.caplen);

    /* write packet header */
    if (write(out_fd, &pkt_header, sizeof(struct pcap_pkthdr)) !=
        sizeof(struct pcap_pkthdr))
    {
        printf("write packet header faile\n");
        //close(in_fd);
        //close(out_fd);
        return EXIT_FAILURE;
    }

    /* write packet */
    printf("%d pkt_header.caplen = 0x%x\n", __LINE__, pkt_header.caplen);
    printf("packet_len = 0x%x\n", packet_len);
    numbytes = write(out_fd, buff, packet_len);
    if (numbytes != packet_len)
    {
        printf("write packet buff failed ! 0x%x\n", numbytes);
        //close(in_fd);
        //close(out_fd);
        return EXIT_FAILURE;
    }

    pcap_dump_buff(packet_len, buff);

        /* update */
    if (lseek(in_fd, packet_len, SEEK_CUR) < 0)
    {
        printf("file end!\n");
        //close(in_fd);
        //close(out_fd);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int pcap_file_close(struct pcap_file_struct *file_des)
{
    int in_fd, out_fd; /* file description */

    in_fd = file_des->infd;
    out_fd = file_des->outfd;

    if (close(in_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    if (close(out_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#if 0
int main(int argc, char **argv)
{
    int i, j;
    int num;
    int in_fd, out_fd; /* file description */
    uint32_t numbytes;
    struct pcap_file_header pcap_header;
    struct pcap_pkthdr pkt_header;
    int flags = O_CREAT | O_TRUNC | O_WRONLY;
    uint8_t buff[2048];
    uint32_t packet_len;

    if (argc < 4)
    {
        pcap_header_usage(argv[0]);
        return EXIT_FAILURE;
    }

    sscanf(argv[3], "%d", &num);
    printf("num=%d\n", num);
    if (!num)
    {
        printf("invalid packet length, must > 0\n");
        return EXIT_FAILURE;
    }

    memset(&pcap_header, 0, sizeof(struct pcap_file_header));
    memset(&pkt_header,  0, sizeof(struct pcap_pkthdr));
    /* open file */
    if ((in_fd = open(argv[1], O_RDONLY, 0644)) < 0) {
        perror("open");
        return EXIT_FAILURE;
    }else{
        printf("opened %s\n",argv[1]);
        printf("descriptor is %d\n", in_fd);
    }

    /* read pacap header */
    if (read(in_fd, &pcap_header, sizeof(struct pcap_file_header))
        != sizeof(struct pcap_file_header))
    {
        printf("read pcap header failed!\n");
        close(in_fd);
    }else {
        printf("pcap_header.magic = 0x%x\n", pcap_header.magic);
        printf("pcap_header.version_major = 0x%x\n", pcap_header.version_major);
        printf("pcap_header.version_minor = 0x%x\n", pcap_header.version_minor);
        printf("pcap_header.thiszone = 0x%x\n", pcap_header.thiszone);
        printf("pcap_header.sigfigs = 0x%x\n", pcap_header.sigfigs);
        printf("pcap_header.snaplen = 0x%x\n", pcap_header.snaplen);
        printf("pcap_header.linktype = 0x%x\n", pcap_header.linktype);
    }

    /* open outfile */
    if ((out_fd = open(argv[2], flags, 0644)) < 0) {
        perror("open");
        close(in_fd); /* have to close both of these now */
        close(out_fd);
        return EXIT_FAILURE;
    }

    /*write pcap header */
    numbytes = write(out_fd, &pcap_header, sizeof(struct pcap_file_header));
    if (numbytes != sizeof(struct pcap_file_header))
    {
        printf("write pcap header failed!\n");
        close(in_fd);
        close(out_fd);
        return EXIT_FAILURE;
    }

    srand( (unsigned)time( NULL ) );
    for (i = 0 ; i < num; i++)
    {
        /* read packet header*/
        if (read(in_fd, &pkt_header, sizeof(struct pcap_pkthdr))
            != sizeof(struct pcap_pkthdr))
        {
            printf("read pcap packet header failed!\n");
            break;
        }else {
            printf("pkt_header.second = 0x%x\n", pkt_header.second);
            printf("pkt_header.micresec = 0x%x\n", pkt_header.micresec);
            printf("pkt_header.caplen = 0x%x\n", pkt_header.caplen);
            printf("pkt_header.len = 0x%x\n", pkt_header.len);
        }

        packet_len = pkt_header.caplen;
        printf("%d pkt_header.caplen = 0x%x\n", __LINE__, pkt_header.caplen);
        memcpy(buff, &pkt_header, sizeof(struct pcap_pkthdr));
        /* write packet header */
        if (write(out_fd, buff, sizeof(struct pcap_pkthdr)) !=
                  sizeof(struct pcap_pkthdr))
        {
            printf("write packet header faile\n");
            close(in_fd);
            close(out_fd);
            return EXIT_FAILURE;
        }

        /* generate date buff */

        /* Display 10 numbers. */
        for( j = 0; j < packet_len; j++)
        {
            buff[j] = rand() & 0xff;
        }
        /* write packet */
        printf("%d pkt_header.caplen = 0x%x\n", __LINE__, pkt_header.caplen);
        printf("packet_len = 0x%x\n", packet_len);
        numbytes = write(out_fd, buff, packet_len);
        if (numbytes != packet_len)
        {
            printf("write packet buff failed ! 0x%x\n", numbytes);
            close(in_fd);
            close(out_fd);
            return EXIT_FAILURE;
        }

        pcap_dump_buff(packet_len, buff);

        /* update */
        if (lseek(in_fd, packet_len, SEEK_CUR) < 0)
        {
            printf("file end!\n");
            close(in_fd);
            close(out_fd);
            return EXIT_FAILURE;
        }
    }

    if (close(in_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    } else {
        printf("closed %s\n",argv[1]);
    }
    if (close(out_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    } else {
        printf("closed %s\n",argv[2]);
    }
    return 0;
}

#else
int main(int argc, char **argv)
{
    int i;
    int num;
    uint8_t buff[2048];
    struct pcap_file_struct file_des;
    if (argc < 4)
    {
        pcap_header_usage(argv[0]);
        return EXIT_FAILURE;
    }

    sscanf(argv[3], "%d", &num);
    printf("num=%d\n", num);
    if (!num)
    {
        printf("invalid packet length, must > 0\n");
        return EXIT_FAILURE;
    }

    /* init */
    if (pcap_file_init(argv[1], argv[2], &file_des))
    {
        printf("pcap_file_init failed!\n");
        return EXIT_FAILURE;
    }
    /* generate packet */
    srand( (unsigned)time( NULL ) );
    for (i = 0; i < 2048; i++)
    {
        buff[i] = rand() & 0xff;
    }

    /* put packet to pacp file */
    for (i = 0; i < num; i++)
    {
        if (pcap_file_add_packet(buff, &file_des))
        {
            printf("pcap_file_add_packet failed\n");
            pcap_file_close(&file_des);
            return EXIT_FAILURE;
        }
    }

    /* close file */
    pcap_file_close(&file_des);
    return EXIT_SUCCESS;
}
#endif
