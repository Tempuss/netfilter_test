#include <stdio.h>
#include "define.h"
#include "libnet-types.h"
#include "libnet-macros.h"
#include "libnet-headers.h"

string block_url;
bool block_check = false;


static u_int32_t print_pkt (struct nfq_data *tb)
{
    int id = 0;
    struct nfqnl_msg_packet_hdr *ph;
    struct nfqnl_msg_packet_hw *hwph;
    u_int32_t mark,ifi;
    int ret;
    unsigned char *data;

    ph = nfq_get_msg_packet_hdr(tb);
    if (ph) {
        id = ntohl(ph->packet_id);
    }

    hwph = nfq_get_packet_hw(tb);
    if (hwph) {
        int i, hlen = ntohs(hwph->hw_addrlen);
    }

    mark = nfq_get_nfmark(tb);


    ret = nfq_get_payload(tb, &data);

    if (ntohs(ph->hw_protocol) == 0x0800)
    {
        const packet* packet_info = (struct packet*)(data);
        const u_char* packet;
        const u_char* http;
        int packet_size = ret;

        int ip_size = 0;
        int tcp_data_size = 0;
        int tcp_size = 0;

        //특정 포트랑 프로토콜만 먼저 선별적으로 필터링
        if (packet_info->ip_header.ip_p == 0x06 && packet_info->tcp_header.th_sport == 0x5000)
        {

            //size 계산
            ip_size = (packet_info->ip_header.header_length&0xF0>>4)*4;
            tcp_size = ((packet_info->tcp_header.header_length&0xF0)>>4)*4;
            tcp_data_size = packet_size - (ip_size + tcp_size);


            //HTTP string
            http = (u_char*)(data+ip_size+tcp_size);

            //정규식 세팅
            regex http_pattern("HTTP");
            regex host_pattern("Host: ");
            string http_str((char*)http);



            string url = "";
            smatch m;

            //HTTP 문자열이 있는지 여부 먼저 체크
            if (regex_search(http_str, m, http_pattern))
            {
                vector<string> http_split = split(http_str, '\n');
                smatch url_match;

                //개행문자로 split 한후 문자열 유무 파싱(보통 첫번째 인덱스에서 나옴
                for(int i=0;i<http_split.size();i++)
                {
                    //HOST 문자열 정규식 검색
                    if (regex_search(http_split[i], url_match, host_pattern))
                    {
                        string url = "";

                        //Host 분리 및 캐리지 리턴 삭제
                        url = replace_all(http_split[i], "Host: ", "");
                        url = url.erase(url.size()-1);

                        //Block URL과 비교
                        if (url.compare(block_url) == 0)
                        {
                            cout<<"BLOCK : ";
                            cout<<block_url<<endl;
                            block_check = true;

                            break;
                        }
                    }
                }
            }
        }
    }


    fputc('\n', stdout);

    return id;
}



static int cb(struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
          struct nfq_data *nfa, void *data)
{
    //값 초기화
    block_check = false;
    u_int32_t id = print_pkt(nfa);

    //차단 유무 체크
    if (block_check == true)
    {
        return nfq_set_verdict(qh, id, NF_DROP, 0, NULL);
    }
    else
    {
        return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
    }
}

int main(int argc, char **argv)
{
    struct nfq_handle *h;
    struct nfq_q_handle *qh;
    struct nfnl_handle *nh;
    block_url = argv[1];
    int fd;
    int rv;
    char buf[4096] __attribute__ ((aligned));

    h = nfq_open();
    if (!h) {
        fprintf(stderr, "error during nfq_open()\n");
        exit(1);
    }

    if (nfq_unbind_pf(h, AF_INET) < 0) {
        fprintf(stderr, "error during nfq_unbind_pf()\n");
        exit(1);
    }

    if (nfq_bind_pf(h, AF_INET) < 0) {
        fprintf(stderr, "error during nfq_bind_pf()\n");
        exit(1);
    }

    qh = nfq_create_queue(h,  0, &cb, NULL);
    if (!qh) {
        fprintf(stderr, "error during nfq_create_queue()\n");
        exit(1);
    }

    if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
        fprintf(stderr, "can't set packet_copy mode\n");
        exit(1);
    }

    fd = nfq_fd(h);

    for (;;) {
        if ((rv = recv(fd, buf, sizeof(buf), 0)) >= 0) {
            //printf("pkt received\n");
            nfq_handle_packet(h, buf, rv);
            continue;
        }
        /* if your application is too slow to digest the packets that
         * are sent from kernel-space, the socket buffer that we use
         * to enqueue packets may fill up returning ENOBUFS. Depending
         * on your application, this error may be ignored. nfq_nlmsg_verdict_putPlease, see
         * the doxygen documentation of this library on how to improve
         * this situation.
         */
        if (rv < 0 && errno == ENOBUFS) {
            printf("losing packets!\n");
            continue;
        }
        perror("recv failed");
        break;
    }

    printf("unbinding from queue 0\n");
    nfq_destroy_queue(qh);

#ifdef INSANE
    /* normally, applications SHOULD NOT issue this command, since
     * it detaches other programs/sockets from AF_INET, too ! */
    printf("unbinding from AF_INET\n");
    nfq_unbind_pf(h, AF_INET);
#endif

    printf("closing library handle\n");
    nfq_close(h);

    exit(0);
}

