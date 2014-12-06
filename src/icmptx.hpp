#ifndef ICMPTX_H
#define ICMPTX_H

#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#define SRC_IP "192.168.0.25"

struct icmp_packet
{
	u_int8_t type;
	u_int8_t code;
	u_int16_t checksum;
	u_int16_t id;
	u_int16_t sequence;
	u_int8_t data[1457];
};

class icmptx
{
	private:
	int rawsock, one;
	uint16_t *ptr;
	int32_t checksum;
	struct sockaddr_in addr;
	unsigned int packetsize;
	unsigned char *packet;
	struct iphdr *ip;
	struct icmp_packet *icmp;
	
	public:
	icmptx();
	~icmptx();
	int sendPacket(char *dst_ip, char *msg, int size);
	
};

#endif
