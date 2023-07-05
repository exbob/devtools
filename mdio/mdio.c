#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/mii.h>
#include <linux/sockios.h>

/*
mdio <dev> <phyid> <r/w> <address> <value>
*/
struct mdio_t{
    char dev[20];
    unsigned char phyid;
    unsigned char dir;
    unsigned long addr;
    unsigned long value;
};

void help()
{
    printf("Usage:\n");
    printf("mdio <dev> <phyid> <r/w> <address> [value]\n");
    printf("     dev: network device name , eg 'eth0'\n");
    printf("     phyid: PHY ID , eg '1~9'\n");
    printf("     r/w: data direction, read or write\n");
    printf("     address: register address, hex\n");
    printf("     value: the value of write to register , hex\n");
}

int getopts(int argc, char **argv, struct mdio_t *arg)
{
    if(argc < 5)
    {
        return -1;
    }

    strncpy(arg->dev, argv[1], 20);
    arg->dev[19] = '\0';

    arg->phyid = atoi(argv[2]);
    arg->dir = argv[3][0]; 
    arg->addr = strtol(argv[4], NULL, 16);

    if(arg->dir == 'w')
    {
        if(argc != 6)
            return -1;
        arg->value = strtol(argv[5], NULL, 16);
    }

    return 0;
}

/*
linux/sockios.h:
#define SIOCGMIIREG 0x8948      // Read MII PHY register.
#define SIOCSMIIREG 0x8949      // Write MII PHY register.

linux/mii.h:
// This structure is used in all SIOCxMIIxxx ioctl calls 
struct mii_ioctl_data {
        __u16           phy_id;
        __u16           reg_num;
        __u16           val_in;
        __u16           val_out;
};
*/

int main(int argc,char **argv)
{
    int ret = 0;
    
    struct mdio_t m;
    struct ifreq ifr;

    ret = getopts(argc, argv, &m);
    if(ret)
    {
        help();
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, m.dev);

    struct mii_ioctl_data* mii = (struct mii_ioctl_data*)(&ifr.ifr_data);
    mii->phy_id  = m.phyid;
    mii->reg_num = m.addr;
    mii->val_out  = 0;
    if(m.dir == 'r')
        mii->val_in = 0;
    else
        mii->val_in = m.value;

    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        printf("socket failed!");
        return -1;        
    }

    if(m.dir == 'r')
    {
        printf("Read from %s.%d\n", m.dev, m.phyid);
        ret = ioctl(fd, SIOCGMIIREG, &ifr);
        if ( ret != -1)
        {            
            printf("Register 0x%04hx = 0x%04hX \n", mii->reg_num, mii->val_out);
        }
        else
        {
            printf("Error!\n");
        }
    }
    else
    {
        printf("Wirte to %s.%d\n", m.dev, m.phyid);
        ret = ioctl(fd, SIOCSMIIREG, &ifr);
        if ( ret != -1)
        {
            printf("Register 0x%04hx = 0x%04hX \n", mii->reg_num, mii->val_in);
        }
        else
        {
            printf("Error!\n");
        }
    }

    close(fd);

    return 0;
}

