#!/usr/bin/env python2.7

import socket
import struct
import os
import random
import sys

'''
import select
import asyncore
import time
'''


ICMP_ECHO_REQUEST = 8
ICMP_ECHO_CODE = 0
i = 6

def getdstIP():
	fd = open(sys.argv[0][:sys.argv[0].rfind("/")] + "/config", "r")
	config_data = fd.read()
	fd.close()
	return config_data[3:config_data.rfind("\n")]


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
	id = random.randint(1, 65535)
	print "[ \033[32mDEBUG\033[0m ] ID     : " + str(id)

	# make dummy header vor calculate the checksum
	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, 0, id, 1)

	# Calculate the checksum on the data and the dummy header.
	my_checksum = icmp_checksum(header + msg_fp)

	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, socket.htons(my_checksum), id, 1)

	connection.sendto(header+msg_fp, (dst_Ip, 0))




#Old Message and Destination.
msg = "Hello World"

'''
dst_ip = raw_input("Type your destination (192.168.2.1) for sending a Message: ")
msg = raw_input("Type your Message: ")'''
print " "

dst_ip = getdstIP()

connection = socket.socket(proto = socket.IPPROTO_ICMP, type = socket.SOCK_RAW)
sendPacket(dst_ip, msg)
print "[ \033[32mDEBUG\033[0m ] DST_IP : " + dst_ip
print "[ \033[32mDEBUG\033[0m ] MSG    : " + str(len(msg)) + " Byte sent"
print " "
datan = connection.recvfrom(100)

print datan[2:]



connection.close()
