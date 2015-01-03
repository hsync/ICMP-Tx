#!/usr/bin/env python2.7

import socket
import string
import struct
import os
import random
import sys


ICMP_ECHO_REQUEST = 8
ICMP_ECHO_RESPONSE = 0
ICMP_ECHO_CODE = 0


#Calc cheksum
def icmp_checksum(source_string):
	sum = 0
	count_to = (len(source_string) / 2) * 2
	count = 0
	while count < count_to:
		this_val = ord(source_string[count + 1])*256+ord(source_string[count])
		sum = sum + this_val
		sum = sum & 0xffffffff
		count = count + 2
	if count_to < len(source_string):
		sum = sum + ord(source_string[len(source_string) - 1])
		sum = sum & 0xffffffff
	sum = (sum >> 16) + (sum & 0xffff)
	sum = sum + (sum >> 16)
	answer = ~sum
	answer = answer & 0xffff
	answer = answer >> 8 | (answer << 8 & 0xff00)
	return answer


def icmptx_sendPacket(dst_Ip, packetID, msg_fp):

	#generate ICMP-Id
	#packetID= random.randint(1, 65535)

	# make dummy header vor calculate the checksum
	header = struct.pack('bbHHh', ICMP_ECHO_RESPONSE, ICMP_ECHO_CODE, 0, packetID, 1)

	# Calculate the checksum on the data and the dummy header.
	my_checksum = icmp_checksum(header + msg_fp)

	header = struct.pack('bbHHh', ICMP_ECHO_RESPONSE, ICMP_ECHO_CODE, socket.htons(my_checksum), packetID, 1)

	icmptx_connection.sendto(header+msg_fp, (dst_Ip, 0))


def icmptx_recvPacket():
	data, addr = icmptx_connection.recvfrom(15000)
	icmp_header = data[20:28]
	itype, icode, checksum, packetID, sequence = struct.unpack("bbHHh", icmp_header)
	host = data[28:]
	host = host[host.rfind("Host: ")+6:]
	host = host[:host.find("\r\n")]


	print "[ \033[32mDEBUG\033[0m ] RX     : Connected with " + addr[0]
	print "[ \033[32mDEBUG\033[0m ] RX     : " + str(len(data[28:])) + " Byte received"
	print "[ \033[32mDEBUG\033[0m ] RX     : Received message content"
	print "[ \033[32mDEBUG\033[0m ] MSG    :\n" + data[28:]
	print "[ \033[32mDEBUG\033[0m ] RX     : Requestet Host"
	print "[ \033[32mDEBUG\033[0m ] MSG    :\n" + host
	msg = 0
#	icmptx_sendPacket(addr[0],packetID,tube_sendPacket(host, data[28:]))
	while(msg[len(msg)-1:] != '\n'):
		msg = tube_sendPacket(host,data[28:])
		icmptx_sendPacket(addr[0],packetID,msg)
def tube_sendPacket(host, data):

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((host,80))
	s.send(data)
	msg = s.recv(15000)
	s.close()
	print "[ \033[32mDEBUG\033[0m ] RX     : Sent message content"
	print "[ \033[32mDEBUG\033[0m ] MSG    :\n" + msg
	return msg

#main program
print "Felix Schulze, Alex Wellnitz 2014 / 2015\n"


#open icmptx socket
icmptx_connection = socket.socket(proto = socket.IPPROTO_ICMP, type = socket.SOCK_RAW)
while 1:
	icmptx_recvPacket()



icmptx_connection.close()



print ""
