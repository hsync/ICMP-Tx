#ifndef GETIP_H
#define GETIP_H

#define NETWORK_DEV_FILE "/proc/net/dev"
#define CONFIG_FILE "/config"

int get_ip(char *dev, char *ip);
int get_dev(int number, char *dev);
int get_valid_ip(int tries, char *ip);
int read_dst_ip(char *argv, char *ip);



#endif
