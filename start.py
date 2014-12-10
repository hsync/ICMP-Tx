#!/usr/bin/env python2.7

import socket
import struct

import select
import random
import asyncore
import time


ICMP_ECHO_REQUEST = 8
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

def sendPacket(dst_Ip, msg_fp):
	id = 5032

	# make dummy header vor calculate the checksum
	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, 0, id, 1)

	# Calculate the checksum on the data and the dummy header.
	my_checksum = icmp_checksum(header + msg_fp)

	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, socket.htons(my_checksum), id, 1)

	connection.sendto(header+msg_fp, (dst_Ip, 0))



msg = "Hello World"
dst = '192.168.178.25'

connection = socket.socket(proto = socket.IPPROTO_ICMP, type = socket.SOCK_RAW)
sendPacket(dst, msg)
print "[ \033[32mDEBUG\033[0m ] DST_IP : " + dst
print "[ \033[32mDEBUG\033[0m ] MSG    : " + str(len(msg)) + " Byte sent"
datan = connection.recvfrom(100)

print datan


connection.close()
