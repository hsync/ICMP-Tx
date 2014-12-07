#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "icmptx.hpp"
#include "getip.h"


int main (int argc, char **argv)
{
	int uid;
	char src_ip[16];
	char dst_ip[16];
	
	char msg[50];

	// application start as root?
	uid =  getuid();
	if(uid != 0)
	{
		printf("You must be root, to run this program\n");
		exit(1);
	}
	
	if(get_valid_ip(5, src_ip) != 0)
	{
		printf("No valid SRC_IP could be found\n");
		exit(2);
	}
	if(read_dst_ip(argv[0], dst_ip) != 0)
	{
		printf("No valid DST_IP could be found\n");
		exit(2);
	}
		
	printf("[\033[32mDEBUG\033[0m] SRC_IP: %s\n", src_ip);
	printf("[\033[32mDEBUG\033[0m] DST_IP: %s\n", dst_ip);
	
	if(argc > 1)
	{
		char *msg = new char[strlen(argv[1])];
		strcpy(msg, argv[1]);
		icmptx packet;
		packet.sendPacket(src_ip, dst_ip, msg, strlen(msg));
	}
	else
	{
		printf("No message\n");
		exit(1);
	}

	return 0;
}


