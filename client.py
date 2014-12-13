#!/usr/bin/env python2.7

import socket
import string
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

def getdstIP():
	
	fd = open(sys.argv[0][:sys.argv[0].rfind("/")] + "/config", "r")
	config_data = fd.readline()
	fd.close()
	return config_data[3:config_data.rfind("\n")]
	

def getProxyPort():
	
	fd = open(sys.argv[0][:sys.argv[0].rfind("/")] + "/config", "r")
	config_data = fd.readlines()[1]
	fd.close()
	return config_data[10:config_data.rfind("\n")]

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


def icmptx_sendPacket(dst_Ip, msg_fp):
	
	#generate ICMP-Id
	packetID = random.randint(1, 65535)

	# make dummy header vor calculate the checksum
	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, 0, packetID, 1)

	# Calculate the checksum on the data and the dummy header.
	my_checksum = icmp_checksum(header + msg_fp)

	header = struct.pack('bbHHh', ICMP_ECHO_REQUEST, ICMP_ECHO_CODE, socket.htons(my_checksum), packetID, 1)

	icmptx_connection.sendto(header+msg_fp, (dst_Ip, 0))
	print "[ \033[32mDEBUG\033[0m ] TX     : " + str(len(msg_fp)) + " Byte sent"


def icmptx_recvPacket():
	data, addr = icmptx_connection.recvfrom(1024)
	icmp_header = data[20:28]
	itype, icode, checksum, packetID, sequence = struct.unpack("bbHHh", icmp_header)
	
	print "[ \033[32mDEBUG\033[0m ] RX     : " + str(len(data[28:])) + " Byte received"
	print "[ \033[32mDEBUG\033[0m ] RX     : Received message content"
	print "[ \033[32mDEBUG\033[0m ] MSG    :\n" + data[28:]
	
	return data[28:]
	
#def proxy_sendPacket():
	
def proxy_recvPacket():
	conn, addr = proxy_connection.accept()
	print "[ \033[32mDEBUG\033[0m ] PROXY  : Source IP " + addr[0]
	print "[ \033[32mDEBUG\033[0m ] PROXY  : Source Port " + str(addr[1])
	while 1:
		data = conn.recv(1024)
		icmptx_sendPacket(getdstIP(), data)
		conn.send(icmptx_recvPacket()) 	
	conn.close()
	


#main program
print " "
dst_ip = getdstIP()
getProxyPort()

#open icmptx socket
icmptx_connection = socket.socket(proto = socket.IPPROTO_ICMP, type = socket.SOCK_RAW)

#open proxy socket
proxy_connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
proxy_connection.bind(('127.0.0.1',string.atoi(getProxyPort())))
proxy_connection.listen(1)


print "[ \033[32mDEBUG\033[0m ] ICMPTX : Server IP " + dst_ip
print "[ \033[32mDEBUG\033[0m ] PROXY  : Use Port " + getProxyPort()



proxy_recvPacket()
icmptx_connection.close()





print ""
