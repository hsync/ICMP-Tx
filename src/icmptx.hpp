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
	int rawsock, one;
	uint16_t *ptr;
	int32_t checksum;
	struct sockaddr_in addr;
	struct sockaddr saddr;
	socklen_t saddr_size;
	int recv_data_size;
	unsigned int packetsize;
	unsigned char *packet;
	struct iphdr *ip;
	struct icmp_packet *icmp;

	public:
	icmptx();
	~icmptx();
	int sendPacket(const char *src_ip, const char *dst_ip, const char *msg, int size);
	int recvPacket();

};

#endif
