#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ssctest.h"

#if 0
int
ss2test_mdio_version(ushort *version)
{
    int rv, fd;
    mdio_ioctl_t arg;



    fd = open(MDIO_DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        printf("Error: Can't open %s.\n", MDIO_DEVICE_NAME);
        return 0;
    }

    rv = ioctl(fd, IOC_VMDIO, &arg);
    if (rv < 0) {
        printf("Error: Ioctl fail!\n");
        close(fd);
        return rv;
    }

    *version = arg.value;

    return 0;
}

int
ss2test_mdio_read(ushort phyid, ushort addr, ushort *value)
{
    int rv, fd;
    mdio_ioctl_t arg;

    fd = open(MDIO_DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        printf("Error: Can't open %s.\n", MDIO_DEVICE_NAME);
        return 0;
    }

    arg.addr = addr;
    arg.phyid = phyid;

    rv = ioctl(fd, IOC_RMDIO, &arg);
    if (rv < 0) {
        printf("Error: Ioctl fail!\n");
        close(fd);
        return rv;
    }

    *value = arg.value;

    return 0;
}

int
ss2test_mdio_write(ushort phyid, ushort addr, ushort value)
{
    int rv, fd;
    mdio_ioctl_t arg;

    fd = open(MDIO_DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        printf("Error: Can't open %s.\n", MDIO_DEVICE_NAME);
        return 0;
    }

    arg.addr = addr;
    arg.phyid = phyid;
    arg.value = value;


    rv = ioctl(fd, IOC_WMDIO, &arg);
    if (rv < 0) {
        printf("Error: Ioctl fail!\n");
        close(fd);
        return rv;
    }

    return 0;
}

int
ss2test_mdio_setting(unsigned char mdc_port, unsigned char mdc_pin, unsigned char mdio_port, unsigned char mdio_pin)
{
    int fd, rv;
    smdio_bus_t arg;

    arg.defined   = 1;
    arg.mdc_port  = mdc_port;
    arg.mdc_pin   = mdc_pin;
    arg.mdio_port = mdio_port;
    arg.mdio_pin  = mdio_pin;
    /*
    printf("%s.%d: arg.mdc_port  = %d\n", __func__, __LINE__, arg.mdc_port  );
    printf("%s.%d: arg.mdc_pin   = %d\n", __func__, __LINE__, arg.mdc_pin   );
    printf("%s.%d: arg.mdio_port = %d\n", __func__, __LINE__, arg.mdio_port );
    printf("%s.%d: arg.mdio_pin  = %d\n", __func__, __LINE__, arg.mdio_pin  );
    */
    fd = open(MDIO_DEVICE_NAME, O_RDWR);
    if(fd < 0) {
        printf("Error: Can't open %s.\n", MDIO_DEVICE_NAME);
        return 1;
    }

    rv = ioctl(fd, IOC_SMDIO, &arg);
    if (rv < 0) {
        printf("Error: Ioctl fail!\n");
    close(fd);
    return rv;
    }

    return 0;
}
#endif

void
cmd_test_usage(void)
{
    printf("%s test (-R|-W|-V|-S) [-p <Phyid>] -a <Addr> [-v <Value>] \n", program);
    printf("                      [-c <MDC_Port>] [-C <MDC_Pin] [-d <MDIO_Port>] [-D <MDIO_Pin>]\n");
    printf("    -- Read/write mdio.\n");
    printf("    -R         Read.\n");
    printf("    -W         Write.\n");
    printf("    -V         Version.\n");
    printf("    -p <Phyid> Phyid; Hex format, Wide: 1 Byte, default: 0.\n");
    printf("    -a <Addr>  Addr of register; Hex format, Wide: 1 Byte, no default.\n");
    printf("    -v <Value> Value to write; Hex format, Wide: 1 Byte, default: 0.\n");
    printf("    MDC_Port,MDIO_Port must be (0 - 3).\n");
    printf("        MDC_Pin, MDIO_Pin  must be <0 - 32>.\n");
}

int
cmd_test(int argc, char **argv)
{
    int opt;
    int action = OP_NULL;
    ushort phyid = 0, addr = 0, value = 0;
    unsigned char mdc_port = 0, mdc_pin = 0, mdio_port = 0, mdio_pin = 0;


    if (argc < 2) {
        usage(argv[0]);
    }

    while ((opt = getopt(argc, argv, "VRWSp:a:v:hc:C:d:D:?")) != -1) {
        switch (opt) {
        case 'V':
            action = OP_VERSION;
            break;
        case 'R':
            action = OP_READ;
            break;
        case 'W':
            action = OP_WRITE;
            break;
        case 'S':
                action = OP_SETTING;
              break;
        case 'p':
            HARGTS(phyid, 0x0, 0xffff);
            break;
        case 'a':
            HARGTS(addr, 0x0, 0xffff);
            break;
        case 'v':
            HARGTS(value, 0x0, 0xffff);
            break;
        case 'c':
                HARGTC(mdc_port, 0, 3);
              break;
        case 'C':
              HARGTC(mdc_pin, 0, 32);
              break;
        case 'd':
              HARGTC(mdio_port, 0, 3);
              break;
        case 'D':
             HARGTC(mdio_pin, 0, 32);
              break;
        case 'h':
        case '?':
        default:
            usage(argv[0]);
            break;
        }
    }

    switch (action) {
    case OP_READ:
        //ERT(ss2test_mdio_read(phyid, addr, &value));
        printf("PHYID 0x%.4x, addr 0x%.4x, value 0x%.4x.\n", phyid, addr, value);
        break;
    case OP_WRITE:
        //ERT(ss2test_mdio_write(phyid, addr, value));
        break;
    case OP_VERSION:
        //ERT(ss2test_mdio_version(&value));
        printf("MDIO driver version: %d\n", value);
        break;
    case OP_SETTING:
            //ERT(ss2test_mdio_setting(mdc_port, mdc_pin, mdio_port, mdio_pin));
            break;
    default:
        usage(argv[0]);
        break;
    }

    return 0;
}
/* End of file */





