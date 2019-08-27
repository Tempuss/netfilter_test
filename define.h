#ifndef DEFINE_H
#define DEFINE_H

#include "pcap.h"

#define ETHER_ADDR_LEN 6
#define MAC_LENTH 6
#define PCAP_OPENFLAG_PROMISCUOUS   1

#include <string>
#include <cstring>
#include <vector>
#include <string>
#include <pcap.h>
#include <ifaddrs.h>
#include <string.h>
#include <arpa/inet.h>
#include "libnet-types.h"
#include "libnet-macros.h"
#include "libnet-headers.h"
#include <iostream>
#include <algorithm>
#include <ifaddrs.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <regex>
#include <cctype>
#include <string>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>            /* for NF_ACCEPT */
#include <errno.h>

#include <libnetfilter_queue/libnetfilter_queue.h>
#include <cctype>
#include <string>
#include <algorithm>



using namespace std;


#define MAC_LENTH 6

#define LIBNET_LIL_ENDIAN = True;

#pragma pack(1)
struct packet {
    //libnet_ethernet_hdr eth_header;
    libnet_ipv4_hdr ip_header;
    libnet_tcp_hdr tcp_header;
};
#pragma pack(8)





void printHex(int length, struct nfq_data* packet);
void printHex(int length, const u_char* packet);
void printHex(int length, unsigned char *packet);
void printHex(int length, uint8_t* packet);
void printHex(int length, uint32_t* packet);


void sendPacket(pcap_t *fp, u_char *packet, int size);
void sendPacket(pcap_t *fp, const u_char *packet, int size);

bool inArray(uint32_t arr[], uint32_t needle);
bool inArray(uint8_t arr[], uint8_t needle);
bool inArray(char *arr[], char *needle);

int inArrayCheck(uint32_t arr[], uint32_t needle);
int inArrayCheck(uint8_t arr[], uint8_t needle);
int inArrayCheck(char *arr[], char *needle);

char *ipToCharIp(uint32_t ip);
string replace_all(const string &message,const string &pattern,const string &replace);
template<typename Out>
void split(string &s, char delim, Out result);
vector<string> split(string &s, char delim);



void dump(unsigned char* buf, int size);
#endif // DEFINE_H
