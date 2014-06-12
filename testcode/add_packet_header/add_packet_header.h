#ifndef ADD_PACKET_HEADER_INCLUDE_H
#define ADD_PACKET_HEADER_INCLUDE_H

#define uchar unsigned char
#define ushort unsigned short
#define uint   unsigned int
typedef struct {
    uchar  major;
    uchar  revision;
    ushort build;
    uchar  reserved[4];
} packet_header_version;

typedef struct {
    ushort year;
    uchar  month;
    uchar  day;
    uchar  hour;
    uchar  minute;
    uchar  second;
    uchar  reserved;
} packet_header_time;

typedef struct {
    uchar                  packet_flag[8];  /* packet header flag */
    packet_header_version  version;         /* packet header version */
    packet_header_time     time;            /* packet header time */
    uchar                  packet_len;      /* packet header length */
    uchar                  reserverd[3];    /* reserver for align */
    uint                   file_len;        /* file length */
    uchar                  md5[32];         /* file md5 value */
} packet_header;

#define READ_BUFF 100
#endif
