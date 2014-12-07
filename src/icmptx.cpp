#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include "icmptx.hpp"

using namespace std;

icmptx::icmptx()
{
	
	one = 1;
	packetsize = sizeof(struct iphdr) + sizeof(struct icmp_packet);
	packet = new unsigned char[packetsize];
	saddr_size = sizeof(saddr);
	
	ip = (struct iphdr *)packet;
	icmp = (struct icmp_packet *)(packet + sizeof(struct iphdr));
	srand(time(NULL));
	memset(packet, 0, packetsize);
	
	
	// Rawsocket erstellen
	if((rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		perror("socket()");
		exit(1);
	}

	if(setsockopt(rawsock , IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) == -1)
	{
		perror("setsockopt()");
		exit(1);
	}


}

icmptx::~icmptx()
{
	close(rawsock);
	delete []packet;
}

int icmptx::sendPacket(const char *src_ip, const char *dest_ip, const char *msg, int size)
{
	ip->version = 4;		// IPv4
	ip->ihl = 5;			// Header Lenght
	ip->id = htonl(rand());	// IP id
	ip->saddr = inet_addr(src_ip);	
	ip->daddr = inet_addr(dest_ip);
	ip->ttl = 128;
	ip->protocol = IPPROTO_ICMP;
	ip->tot_len = packetsize;
	ip->check = 0;			// Bei 0 wird die Prüfsumme vom Kernel berechnet
	
	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->id = rand();
	icmp->sequence = rand();

	// build packet
	memset((char *)&icmp->data, 0, sizeof(icmp->data));
	strncpy((char *)&icmp->data, msg, size);
	packetsize = sizeof(struct iphdr) + sizeof(struct icmp_packet) - sizeof(icmp->data) + size;
	
	// checksum calc.
	ptr = (u_int16_t *)icmp;
	checksum = 0;
	for(unsigned int i = 0; i < (sizeof(struct icmp_packet)/2); i++)
		checksum += 0xFFFF & (*ptr ++);

	checksum = ~((checksum & 0xFFFF) + ((checksum >> 16) & 0xFFFF));

	icmp->checksum = checksum;
	addr.sin_family = AF_INET;

	// send packet
	if((sendto(rawsock, packet, packetsize, 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in))) == -1)
	{
		perror("sendto()");
		return(1);
	}
	
	return 0;
}

int icmptx::recvPacket()
{
	char buffer[1200];
	recv_data_size = recvfrom(rawsock , buffer , 1200 , 0 , &saddr , &saddr_size);
	
	/* prüfe ob icmp packet, prüfe richtige src ip*/
      
	printf("[\033[32mDEBUG\033[0m] MSG   : %i Byte Data received\n", recv_data_size -28);
    printf("Print Buffer: ");

	for(int i = 0; i<=recv_data_size; i++)
	{
		printf("0x%02x ",(unsigned int8_t)buffer[i]);
	} 
	
	printf("\n\nPrint Buffer: ");
	for(int i = 0; i<=recv_data_size; i++)
	{
		if (buffer[i] > 32 && buffer[i] < 127)
			printf("%c ",buffer[i]);
		else 
			printf("..");
	} 
	
	 printf("\n\n");
 }
           
