#include <stdio.h>
#include "define.h"
#include "libnet-types.h"
#include "libnet-macros.h"
#include "libnet-headers.h"
#include <cctype>
#include <string>
#include <algorithm>



template<typename Out>
void split(string &s, char delim, Out result)
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim))
        *(result++) = item;
}


vector<string> split(string &s, char delim)
{
    vector<string> elem;
    split(s, delim, back_inserter(elem));

    return elem;
}

std::string replace_all(
     const std::string &message,
     const std::string &pattern,
     const std::string &replace
) {
    std::string result = message; std::string::size_type pos = 0;

    while ((pos = result.find(pattern)) != std::string::npos)
    {
        result.replace(pos, pattern.size(), replace);
    }
    return result;
}


int main(int argc, char* argv[]) {

    if (argc != 3) {
      return -1;
    }


    char* dev = argv[1];
    string block_url = argv[2];
    //소문자 변환
    transform(block_url.begin(), block_url.end(), block_url.begin(),[](unsigned char c){ return tolower(c); });

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
      fprintf(stderr, "couldn't open device %s: %s\n", dev, errbuf);
      return -1;
    }


    while (true) {
      struct pcap_pkthdr* header;
      const packet* packet_info;
      const u_char* packet;
      u_char* http;

      int res = pcap_next_ex(handle, &header, &packet);
      int packet_size = header->len;

      int eth_size = sizeof(struct libnet_ethernet_hdr);
      int ip_size = 0;
      int tcp_data_size = 0;
      int tcp_size = 0;


      if (res == 0) continue;
      if (res == -1 || res == -2) break;


      packet_info = (struct packet*)(packet);


      //Check Next IP Header
      if (packet_info->eth_header.ether_type == 0x8 && packet_info->ip_header.ip_p == 0x06 && packet_info->tcp_header.th_sport == 0x5000)
      {
          //size 계산
          ip_size = (packet_info->ip_header.header_length&0xF0>>4)*4;
          tcp_size = ((packet_info->tcp_header.header_length&0xF0)>>4)*4;
          tcp_data_size = packet_size - (eth_size+ip_size + tcp_size);


          http = (u_char*)(packet+eth_size+ip_size+tcp_size);
          
          regex http_pattern("HTTP");
          regex host_pattern("Host: ");
          string http_str((char*)http);
          string url = "";
          smatch m;

          if (regex_search(http_str, m, http_pattern))
          {
              vector<string> http_split = split(http_str, '\n');
              smatch url_match;
              for(int i=0;i<http_split.size();i++)
              {
                  if (regex_search(http_split[i], url_match, host_pattern))
                  {
                        std::string message = "test message";
                        string url = "";

                        //Host 분리 및 캐리지 리턴 삭제
                        url = replace_all(http_split[i], "Host: ", "");
                        url = url.erase(url.size()-1);

                        //Block URL과 비교
                        if (url.compare(block_url) == 0)
                        {
                            cout<<"SAME"<<endl;
                            cout<<url<<endl;
                        }

                  }
              }
          }

      }
      printf("\n");
    }

}
