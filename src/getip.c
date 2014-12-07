#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "getip.h"


int get_ip(char *dev, char *ip)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, dev, IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	/* save result */
	strcpy(ip, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	return 0;
}

int get_dev(int number, char *dev)
{
	number += 2;
	FILE *fp = NULL;

	fp = fopen(NETWORK_DEV_FILE, "r");

    if(fp == NULL)
    {
		printf("File could not be opened\nIp address could not be determined\n");
    }

	for(unsigned i = 0; i < number; i++)
        	while(fgetc(fp) != '\n');

    for(int i = 0; i < 10; i++)
    {
		dev[i] = fgetc(fp);
        if(dev[i] == ':')
		{
			dev[i] = 0;
			break;
        }
    }

   fclose(fp);

	return 0;
}

// Function tries to find a valid Ip for the ethernet devices
int get_valid_ip(int tries, char *ip)
{
	char ip_buffer[16];
	char dev_buffer[16];
	
	for(int i=0; i<tries; i++)
	{
		get_dev(i, dev_buffer);
		get_ip(dev_buffer, ip_buffer);
		if((strcmp(ip_buffer,"0.0.0.0") != 0) && (strcmp(ip_buffer, "127.0.0.1"))) 
		{
			strcpy(ip, ip_buffer);
			return 0;
		}
	}
	return 1;
}
		
int read_dst_ip(char *argv, char *ip)
{
	FILE *fd = NULL;
	char ip_buffer[16];
	char path[80];
	int i = 0;
	
	strcpy(path, argv);
	i = strlen(path);
	
	while(path[i] != '/')
		i--;
		
	path[i] = 0;
	strcat(path, CONFIG_FILE);
	
	
	fd = fopen(path, "r");
	
    if(fd == NULL)
    {
		printf("File could not be opened\ndst_ip address could not be read\n");
    }

	
   while(fgetc(fd) != ':');

    for(int i = 0; i < 16; i++)
    {
		ip_buffer[i] = fgetc(fd);
        if(ip_buffer[i] == '\n')
		{
			ip_buffer[i] = 0;
			strcpy(ip, ip_buffer);
			return 0;
        }
    }
    
    return 1;
}

