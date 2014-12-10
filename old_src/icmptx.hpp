#ifndef ICMPTX_H
#define ICMPTX_H

#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


struct icmp_packet
{
	u_int8_t type;
	u_int8_t code;
	u_int16_t checksum;
	u_int16_t id;
	u_int16_t sequence;
	u_int8_t data[1024];
};

class icmptx
{
	private:
	uint16_t *ptr;
	unsigned int packetsize;
	unsigned char *packet;
	int32_t checksum;
	int rawsock, one;
	struct sockaddr_in addr;
	struct iphdr *ip;
	struct icmp_packet *icmp;
	
	int recv_data_size;
	char recv_buffer[1200];
	struct in_addr ip_addr;
	struct iphdr *iph;
	struct sockaddr saddr;
	socklen_t saddr_size;

	public:
	icmptx();
	~icmptx();
	int sendPacket(const char *src_ip, const char *dst_ip, const char *msg, int size);
	int recvPacket();

};

#endif
