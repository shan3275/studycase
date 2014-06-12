#include <pthread.h>

#include "rawsock.h"

#define ET_ERROR(_fmt, _args...) printf("[ERROR] %s.%d: " _fmt, __func__, __LINE__, ##_args)

#define PACK_U8(_buf, _var) \
        *_buf++ = (_var) & 0xff
#define UNPACK_U8(_buf, _var) \
        _var = *_buf++ & 0xff

#define PACK_U16(_buf, _var) \
        *_buf++ = ((_var) >> 8) & 0xff; \
        *_buf++ = (_var) & 0xff;
#define UNPACK_U16(_buf, _var) \
        _var  = (*_buf++ & 0xff) << 8; \
        _var |= (*_buf++ & 0xff);

#define PACK_U24(_buf, _var) \
        *_buf++ = ((_var) >> 16) & 0xff; \
        *_buf++ = ((_var) >> 8) & 0xff; \
        *_buf++ = (_var) & 0xff;

#define UNPACK_U24(_buf, _var) \
        _var = 0; \
        _var |= (*_buf++ & 0xff) << 16; \
        _var |= (*_buf++ & 0xff) << 8; \
        _var |= (*_buf++ & 0xff);

#define PACK_U32(_buf, _var) \
        *_buf++ = ((_var) >> 24) & 0xff; \
        *_buf++ = ((_var) >> 16) & 0xff; \
        *_buf++ = ((_var) >> 8) & 0xff; \
        *_buf++ = (_var) & 0xff;

#define UNPACK_U32(_buf, _var) \
        _var = 0; \
        _var |= (*_buf++ & 0xff) << 24; \
        _var |= (*_buf++ & 0xff) << 16; \
        _var |= (*_buf++ & 0xff) << 8; \
        _var |= (*_buf++ & 0xff);

#define PACK_BYTES(_buf, _var, _len) \
{ \
    int b; \
    for (b = 0; b < _len; b++) { \
        PACK_U8(_buf, (_var)[b]); \
    } \
}

#define UNPACK_BYTES(_buf, _var, _len) \
{ \
    int b; \
    for (b = 0; b < _len; b++) { \
        UNPACK_U8(_buf, (_var)[b]); \
    } \
}

#define PACK_MAC(_buf, _mac) \
        *_buf++ = _mac[0] & 0xff; \
        *_buf++ = _mac[1] & 0xff; \
        *_buf++ = _mac[2] & 0xff; \
        *_buf++ = _mac[3] & 0xff; \
        *_buf++ = _mac[4] & 0xff; \
        *_buf++ = _mac[5] & 0xff;
#define UNPACK_MAC(_buf, _mac) \
        _mac[0] = *_buf++ & 0xff; \
        _mac[1] = *_buf++ & 0xff; \
        _mac[2] = *_buf++ & 0xff; \
        _mac[3] = *_buf++ & 0xff; \
        _mac[4] = *_buf++ & 0xff; \
        _mac[5] = *_buf++ & 0xff;

typedef enum {
    ETRANS_FLAG_LOOPBACK,
    ETRANS_FLAG_DUMP_RBUF,
    ETRANS_FLAG_DUMP_TBUF,
} etrans_flag_e;

typedef struct {
    rawsock_desc_t rawsock;
    mac_t smac;
    mac_t dmac;
    pthread_t recv_thread;
    pthread_t send_thread;
    pthread_t stat_thread;
    uint32_t etrans_debug_flag;
    uint64_t rx_pkts;
    uint64_t rx_errs;
    uint64_t rx_byts;
    uint64_t tx_pkts;
    uint64_t tx_errs;
    uint64_t tx_byts;
    uint8_t recv_buff[RAWSOCKET_PKT_BUFF_LEN];
    uint8_t send_buff[RAWSOCKET_PKT_BUFF_LEN];
    uint16_t send_len;
    uint16_t proto;
    uint32_t flags;
} etrans_handler_t;


#define ETRANS_FLAG_SET(_handler, _flag) (((_handler)->flags) |= (1 << _flag))
#define ETRANS_FLAG_GET(_handler, _flag) (((_handler)->flags) & (1 << _flag))



char *program = NULL;

int
parse_mac(char *str, mac_t mac)
{
    int rv;
    int m0, m1, m2, m3, m4, m5;

    rv = sscanf(str, "%2x:%2x:%2x:%2x:%2x:%2x",
                &m0, &m1, &m2, &m3, &m4, &m5);

    if (rv != 6) {
        return 1;
    }

    mac[0] = (unsigned char) m0;
    mac[1] = (unsigned char) m1;
    mac[2] = (unsigned char) m2;
    mac[3] = (unsigned char) m3;
    mac[4] = (unsigned char) m4;
    mac[5] = (unsigned char) m5;

    return 0;
}


int usage()
{
    printf("USAGE: %s INTERFACE PROTOCOL DMAC LENGTH\n", program);
    printf("       %s INTERFACE PROTOCOL loopback\n", program);

    exit(1);
}


int get_if_ethaddr(char *devname, mac_t mac)
{
    int s, err;
    struct ifreq req;

    s = socket(AF_INET,SOCK_DGRAM,0);
    strcpy(req.ifr_name, devname);

    err = ioctl(s, SIOCGIFHWADDR, &req);

    close(s);

    if(err == -1)
    {
        return 1;
    }

    memcpy(mac, req.ifr_hwaddr.sa_data, 6);

    return 0;
}


int parse_u32(char *str, uint32_t *value)
{
    char *e;
    if (!strncasecmp("0x", str, 2))
    {
        *value = strtoul(str, &e, 16);
    }
    else
    {
        *value = strtoul(str, &e, 10);
    }

    if (e == (str) || *e != '\0') {
        return 1;
    }

    return 0;
}

int parse_u16(char *str, uint16_t *value)
{
    int rv;

    uint32_t v32 = 0;

    rv = parse_u32(str, &v32);

    if (rv)
    {
        return 1;
    }

    if (v32 > 0xffff)
    {
        return 1;
    }
    *value = v32 & 0xffff;

    return 0;
}

int parse_u8(char *str, uint8_t *value)
{
    int rv;

    uint32_t v32 = 0;

    rv = parse_u32(str, &v32);

    if (rv)
    {
        return 1;
    }

    if (v32 > 0xff)
    {
        return 1;
    }

    *value = v32 & 0xff;

    return 0;
}


void dump_buff(uint8_t *buff, int len)
{
    int i;

    if (buff == NULL)
    {
        printf("Buff is NULL, len %d.\n", len);
        return;
    }

    printf("  Buff %p, len %d: \n", buff, len);
    for (i = 0; i < len; i++)
    {
        if ((i % 16) == 0)
        {
            printf("  %.3xH:", i);
        }
        if ((i % 16) == 8)
        {
            printf(" ");
        }
        printf(" %.2x", buff[i] & 0xff);
        if ((i % 16) == 15)
        {
            printf("\n");
        }
    }
    printf("\n");
}

int etrans_send_buff(etrans_handler_t *handler, uint8_t *send_buff, int len)
{
     int send_len;
     if (ETRANS_FLAG_GET(handler, ETRANS_FLAG_DUMP_TBUF))
     {
         printf("%s.%d ", __func__, __LINE__);
         dump_buff(handler->send_buff, len);
     }

     send_len = rawsock_send(&handler->rawsock, (char *) send_buff, handler->send_len);
     printf("send_len=%d  len=%d\n", send_len, len);
     if (send_len != len)
     {
         handler->tx_errs++;
         return 1;
     }

     handler->tx_pkts++;
     handler->tx_byts += send_len;

     return 0;
}


void *etrans_recv_loop(void *arg)
{
    int recv_len;

    etrans_handler_t *handler = (etrans_handler_t *) arg;

    while(1)
    {
         recv_len = rawsock_recv(&handler->rawsock, (char *) handler->recv_buff);
         if (ETRANS_FLAG_GET(handler, ETRANS_FLAG_DUMP_RBUF))
         {
             printf("_%s.%d ", __func__, __LINE__);
             dump_buff(handler->recv_buff, recv_len);
         }

         handler->rx_pkts++;
         handler->rx_byts += recv_len;

         if (ETRANS_FLAG_GET(handler, ETRANS_FLAG_LOOPBACK))
         {
             etrans_send_buff(handler, handler->recv_buff, recv_len);
         }
    }

    return NULL;
}



void *etrans_send_loop(void *arg)
{
    etrans_handler_t *handler = (etrans_handler_t *) arg;

    while(1)
    {
         etrans_send_buff(handler, handler->send_buff, handler->send_len);
         //sleep(1);
     //usleep(1);
    }

    return NULL;
}

void speed_string(uint64_t old, uint64_t cur, char str[30])
{
    uint64_t speed = 0;
    speed = cur - old;
    memset(str, 0, 30);


    if (speed > (1000 * 1000))
    {
        sprintf(str, "%15lld/%-lldMbps", (ULL) cur, (ULL)(speed / (1000 * 1000)));
    }
    else if (speed > 1000)
    {
        sprintf(str, "%15lld/%-lldKbps", (ULL) cur, (ULL)(speed / 1000));
    }
    else
    {
        sprintf(str, "%15lld/%-lldbps", (ULL) cur, (ULL) speed );
    }

}

void *etrans_stat_loop(void *arg)
{
    //struct timeval start, now;
    int sec = 0;
    uint64_t old_rx_pkts = 0, old_rx_byts = 0, old_tx_pkts = 0, old_tx_byts = 0;
    uint64_t cur_rx_pkts = 0, cur_rx_byts = 0, cur_tx_pkts = 0, cur_tx_byts = 0;
    char rx_pkts_speed[30], rx_byts_speed[30], tx_pkts_speed[30],tx_byts_speed[30];

    etrans_handler_t *handler = (etrans_handler_t *) arg;

    //gettimeofday(&start, NULL);
    printf("%25s %25s %25s %25s %15s %6s\n", "RX PKTS/SPEED", "RX BYTS/SPEED", "TX PKTS/SPEED", "TX BYTS/SPEED", "TX_ERRS", "SPEND");

    while(1)
    {
        cur_rx_pkts = handler->rx_pkts;
        cur_rx_byts = handler->rx_byts;
        cur_tx_pkts = handler->tx_pkts;
        cur_tx_byts = handler->tx_byts;
#if 1
        speed_string(old_rx_pkts, cur_rx_pkts, rx_pkts_speed);
        speed_string(old_rx_byts * 8, cur_rx_byts * 8, rx_byts_speed);
        speed_string(old_tx_pkts, cur_tx_pkts, tx_pkts_speed);
        speed_string(old_tx_byts * 8, cur_tx_byts * 8, tx_byts_speed);
#endif
        old_rx_pkts = cur_rx_pkts;
        old_rx_byts = cur_rx_byts;
        old_tx_pkts = cur_tx_pkts;
        old_tx_byts = cur_tx_byts;

        printf("\r%25s %25s %25s %25s %15lld %6d",
               rx_pkts_speed, rx_byts_speed, tx_pkts_speed, tx_byts_speed, (ULL) handler->tx_errs, sec);
        fflush(stdout);
        sec++;

        sleep(1);

    }

    return NULL;
}


void etrans_send_buff_gen(etrans_handler_t *handler)
{
    int i;
    uint8_t *p = NULL;

    p = handler->send_buff;

    PACK_MAC(p, handler->dmac);
    PACK_MAC(p, handler->smac);
    PACK_U16(p, handler->proto);

    for (i = 0; i < (handler->send_len - 14); i++)
    {
        *p++ = i * 0xff;
    }
}

int main(int argc, char **argv)
{

    etrans_handler_t handler;

    program = argv[0];

    if (argc < 4)
    {
        usage();
    }
    memset(&handler, 0, sizeof(etrans_handler_t));

    if (parse_u16(argv[2], &handler.proto))
    {
         usage();
        return 1;
    }

    if (argc == 5)
    {

        if(parse_mac(argv[3], handler.dmac))
        {
             usage();
            return 1;
        }


        if (parse_u16(argv[4], &handler.send_len))
        {
             usage();
            return 1;
        }
    }
    else if (argc == 4)
    {
        if (!strncasecmp("loopback", argv[3], strlen(argv[3]) < strlen("loopback") ? strlen(argv[3]) : strlen("loopback")))
        {
            ETRANS_FLAG_SET(&handler, ETRANS_FLAG_LOOPBACK);
        }
    }

    if (rawsock_open(&handler.rawsock, argv[1], handler.proto)) {
        ET_ERROR("Can't open interface %s\n", argv[1]);
        return 1;
    }

    if (get_if_ethaddr(argv[1], handler.smac))
    {
        ET_ERROR("Get %s ethaddr fail!\n", argv[1]);
        return 1;
    }

    handler.recv_thread = pthread_create(&handler.recv_thread, NULL, etrans_recv_loop, (void *)&handler);

    if (handler.recv_thread != 0)
    {
        ET_ERROR("Create recv thread fail!\n");
        return 1;
    }

    if (!ETRANS_FLAG_GET(&handler, ETRANS_FLAG_LOOPBACK))
    {
        etrans_send_buff_gen(&handler);

        handler.send_thread = pthread_create(&handler.send_thread, NULL, etrans_send_loop, (void *)&handler);

        if (handler.send_thread != 0)
        {
            ET_ERROR("Create send thread fail!\n");
            return 1;
        }
    }

    handler.stat_thread = pthread_create(&handler.stat_thread, NULL, etrans_stat_loop, (void *)&handler);

    if (handler.stat_thread != 0)
    {
        ET_ERROR("Create stat thread fail!\n");
        return 1;
    }

    while (1)
    {
        sleep(30);
    }

    return 0;
}

/* End of file */
