//#define _GNU_SOURCE
#include "common.h"
#include "filter_rule_main.h"
#include "pkt_parser.h"

int get_ipstr(const struct pfring_pkthdr* hdr, char* srcIP)
{
        u_int32_t ip = 0;
        if(hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id != NO_TUNNEL_ID)
        {
                ip = hdr->extended_hdr.parsed_pkt.tunnel.tunneled_ip_src.v4;
        }
        else
        {
                ip = hdr->extended_hdr.parsed_pkt.ipv4_src;
        }
        if (0 != ip)
        {
                inet_ntoa_r(ip, srcIP);
                return 0;
        }
        else
        {
                return -1;
        }
}

/*main filter loop*/
int http_cap_filter_main(int thread_id, u_char *buffer, struct pfring_pkthdr* hdr)
{
	if(NULL == buffer)
	{
		return -1;
	}
    int ret = 0;
    struct http_request_kinfo http;

    //1、parser http offset
    ret = http_parser_level_1(thread_id, hdr, buffer, &http);
    if(ret != 0)
    {
        //LOG_ERROR("error", "level 1 error: %d\n", ret);
        return ret;
    }


    //2、parser http header
    ret = http_parser_level_2( thread_id, hdr, buffer, &http);
    if(ret != 0)
    {
        //LOG_ERROR("error", "level 2 error: %d\n", ret);
        return ret;
    }

    // 业务处理了下面   
    return 0;
}
