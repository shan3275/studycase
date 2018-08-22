#include "pkt_parser.h"


FILE * ffp = NULL;

/*
 * transfer int to str, should be very fast
 */
void http_parser_int2str(u_int num, char **str)
{
	int i = 0;
	char temp[64] = {0};
	while (num)
	{
		temp[i] = num % 10 + '0';
		num = num / 10;
		i++;
	}
	temp[i] = 0;
    i = i - 1;
    while (i >= 0)
	{
		*(*str)++ = temp[i];
		i--;
	}
}
void http_parser_int2str_with_wd(u_int num, char **str, int width)
{
    int i = 0;
    char temp[64] = {0};
    while (num)
    {
        temp[i] = num % 10 + '0';
        num = num / 10;
        i++;
    }
    while(width > i)
    {
        width--;
        *(*str)++ = '0';
    }
    temp[i] = 0;
    i = i - 1;
    while (i >= 0)
    {
        *(*str)++ = temp[i];
        i--;
    }

}
int http_parser_atoi(const char * start, const char * end)
{
    char s[16];
    char * to_s = s;
    while(start<=end)
    {
        *to_s++ = *start++;
    }
    *to_s = '\0';
    return atoi(s);
}
/*
 * append str to another str, should be very fast
 */
void http_parser_str_append(char ** dst, const char * str)
{
    const char * append_str = str;
    while(*append_str != '\0')
    {
        **dst = *append_str;
        (*dst)++;
        append_str++;
    }
}
/*
 * transfer http struct to json string
 */
int http_parser_level_3(struct http_request_kinfo *httprequest, char *str, struct pfring_pkthdr *hdr)
{
	u_char *p;
	u_int num = 0;
	int urilen = 0, hostlen = 0, referlen = 0, ualen = 0, cookieslen = 0;

	urilen = httprequest->uri_end - httprequest->uri_start;
	hostlen = httprequest->host_end - httprequest->host_start;

    referlen = httprequest->referer_end - httprequest->referer_start;
	ualen = httprequest->ua_end - httprequest->ua_start;
	cookieslen = httprequest->cookies_end - httprequest->cookies_start;

    int post_flag = 0;

    /*check para*/
    if((hostlen > 100) || (hostlen < 3))
    {
        printf("struct2json: hostlen = %d\n.", hostlen);
        return -1;
    }
    /* url */
    if((httprequest->uri_start > 0) && (hostlen > 0))
	{
        http_parser_str_append(&str, "http://");
        for (p = httprequest->host_start; p <= httprequest->host_end;
                p++, str++)
        {
            *str = *p;
        }
		for (p = httprequest->uri_start; p <= httprequest->uri_end; p++, str++)
		{
            if (*p != 34)
			{ *str = *p;}

		else
		{
			*str = ' ';
		}}
		//*str++ = '';
	}
	else
	{
        	printf("struct2json: no url.");
		return -1;
	}

    /* user agent */
	if (ualen > 0)
	{
        http_parser_str_append(&str, "\t");
		for (p = httprequest->ua_start; p <= httprequest->ua_end; p++, str++)
		{
            if (*p == 34)
			{*str = ' ';}
            else
            {*str = *p;}
		}
		//*str++ = '\"';
	}

    /* cookies */
    if (cookieslen > 0)
    {
        http_parser_str_append(&str, "\t");
        for (p = httprequest->cookies_start; p <= httprequest->cookies_end;
             p++, str++)
        {
            if (*p == 34)
            {*str = ' ';}
            else
            {*str = *p;}
        }

        //*str++ = '\"';
    }

    /* host */
	if (hostlen > 0)
	{
        http_parser_str_append(&str, "\t");
		for (p = httprequest->host_start; p <= httprequest->host_end;
				p++, str++)
		{
			*str = *p;
		}
		//*str++ = '\"';
	}

    /* referer */
	if (referlen > 0)
	{
        http_parser_str_append(&str, "\t");
		for (p = httprequest->referer_start; p <= httprequest->referer_end;
				p++, str++)
		{
            if (*p == 34)
            {*str = ' ';}
            else
            {*str = *p;}
		}
		//*str++ = '\"';
	}

	/* srcip */
	{
        http_parser_str_append(&str, "\t");
		num = httprequest->sip;
        http_parser_int2str(num, &str);
		//*str++ = '\"';
	}

    /* dstip */
	{
        http_parser_str_append(&str, "\t");
		num = httprequest->dip;
        http_parser_int2str(num, &str);
		//*str++ = '\"';
	}
    /* ts */
	{
        http_parser_str_append(&str, "\t");
        http_parser_int2str((u_int)httprequest->sec, &str);
        http_parser_int2str_with_wd((u_int)httprequest->usec/1000, &str,3);
		//*str++ = '\"';
	}
    //*str++ = '}';
    *str++ = '\n';
    *str = '\0';
	return 0;
}


/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void http_parser_print_hex_ascii_line(const u_char *payload, int len, int offset)
{
	int i;
	int gap;
	const u_char *ch;

	/* offset */
	printf("%05d   ", offset);
	/* hex */
	ch = payload;
	for (i = 0; i < len; i++)
	{
		printf("%02x ", *ch);
		ch++;
		/* print extra space after 8th byte for visual aid */
		if (i == 7)
			printf(" ");
	}
	/* print space to handle line less than 8 bytes */
	if (len < 8)
		printf(" ");
	/* fill hex gap with spaces if not full line */
	if (len < 16)
	{
		gap = 16 - len;
		for (i = 0; i < gap; i++)
		{
			printf("   ");
		}
	}
	printf("   ");
	/* ascii (if printable) */
	ch = payload;
	for (i = 0; i < len; i++)
	{
		if (isprint(*ch))
			printf("%c", *ch);
		else
			printf(".");
		ch++;
	}
	printf("\n");
	return;
}

/*
 * print packet payload data (avoid printing binary data)
 */
void http_parser_print_payload(const u_char *payload, int len)
{

	int len_rem = len;
	int line_width = 16; /* number of bytes per line */
	int line_len;
	int offset = 0; /* zero-based offset counter */
	const u_char *ch = payload;

	if (len <= 0)
		return;

	/* data fits on one line */
	if (len <= line_width)
	{
        http_parser_print_hex_ascii_line(ch, len, offset);
		return;
	}

	/* data spans multiple lines */
	for (;;)
	{
		/* compute current line length */
		line_len = line_width % len_rem;
		/* print line */
        http_parser_print_hex_ascii_line(ch, line_len, offset);
		/* compute total remaining */
		len_rem = len_rem - line_len;
		/* shift pointer to remaining bytes to print */
		ch = ch + line_len;
		/* add offset */
		offset = offset + line_width;
		/* check if we have line width chars or less */
		if (len_rem <= line_width)
		{
			/* print last line and get out */
            http_parser_print_hex_ascii_line(ch, len_rem, offset);
			break;
		}
	}
	return;
}


void print(u_char* ucParam, int nlen)
{
	printf("\nfalg1\n"); 
         int i;
         for(i=0; i<nlen; ++i)
         {
                printf(" %02x", ucParam[i]);
                if( (i + 1) % 16 == 0 )
                {
                        printf("\n");
                }
         }
         printf("\n\n");
}

/*
 * parser eth packet buffer to struct
 */
int http_parser_level_2(struct pfring_pkthdr * hdr, u_char *buffer, struct http_request_kinfo *httprequest)
{;
    u_char ch, *t, *m = NULL;
    int i = 0;

    u_char * p = httprequest->content;
    const int len = httprequest->len;
    enum
    {
        sw_start = 0,
        sw_method,
        sw_spaces_before_uri,
        sw_uri,
        sw_http_09,
        sw_spaces_after_digit,
        sw_head_start,
        sw_name,
        sw_space_before_value,
        sw_value,
        sw_space_after_value,
        sw_ignore_line,
        sw_almost_done
    } state;

    state = sw_start;
    for (i = 0; ((i < len) && (*p!='\0')); i++, p++)
    {
        ch = *p;

        switch (state)
        {
        /* HTTP methods: GET */
        case sw_start:
            m = p;
            if (ch == CR || ch == LF)
            {
                break;
            }

            if (ch < 'A' || ch > 'Z')
            {
                return 998;
            }
            state = sw_method;
            break;
        case sw_method:

            if (ch == ' ')
            {
                if (((p - m) == 3) && str3_cmp(m, 'G', 'E', 'T', ' '))
                {
                    state = sw_spaces_before_uri;
                    break;
                }
                else if (((p - m) == 4) && str5cmp(m, 'P', 'O', 'S', 'T', ' '))
                {
                	httprequest->http_type = 1;
                    state = sw_spaces_before_uri;
                    break;
                }
                else
                {
                    return 12;
                }
            }
            if (ch < 'A' || ch > 'Z')
            {
                return 13;
            }
            break;
        case sw_spaces_before_uri:

            if (ch == '/')
            {
                m = p;
                httprequest->uri_start = p;
                state = sw_uri;
                break;
            }
            switch (ch)
            {
            case ' ':
                break;
            default:
                return 14;
            }
            break;

        case sw_uri:

            switch (ch)
            {
            case ' ':
                httprequest->uri_end = p - 1;
                t = httprequest->uri_end;
                m = (t - m > 10) ? (t - 10) : m;
                state = sw_http_09;
                break;
            case '?':
                t = p - 1;
                m = (t - m > 10) ? (t - 10) : m;
                break;
            case CR :
                return 15;
            case LF :
                return 16;
            case '\0':
                return 17;
            default:
                break;
            }
            break;

        case sw_http_09:

            switch (ch)
            {
            case ' ':
                break;
            case CR :
                return 18;
            case LF :
                return 19;
            case 'H':
                state = sw_spaces_after_digit;
                break;
            default: //space in uri;
                state = sw_uri;
                break;
            }
            break;
        case sw_spaces_after_digit:
            switch (ch)
            {
            case ' ':
                break;
            case CR :
                break;
            case LF :
                state = sw_head_start;
                break;
            }
            break;
        case sw_head_start:
            switch (ch)
            {
            case ' ':
                break;
            case CR :
                state = sw_almost_done;
                break;
            case LF :
            	httprequest->post_start = p+1;
                goto request_done;
            default:
                if (ch == '\0')
                {
                    return 21;
                }
                m = p;
                state = sw_name;
                break;
            }
            break;
        case sw_name:

            if (ch == ' ' || ch == ':')
            {

                switch (p - m)
                {

                case 4: //Host
                    if (str4cmp(m, 'H', 'o', 's', 't'))
                    {
                        state = sw_space_before_value;
                        httprequest->header_state = HS_HOST;
                        break;
                    }
                    state = sw_space_after_value;
                    break;

                case 6: //Cookie
                    if (str6cmp(m, 'C', 'o', 'o', 'k', 'i', 'e'))
                    {
                        state = sw_space_before_value;
                        httprequest->header_state = HS_COOKIE;
                        httprequest->cookies_start = p+1;
                        httprequest->cookies_end = buffer + hdr->caplen;
                        break;
                    }
                    state = sw_space_after_value;
                    break;
                case 7: //Referer
                    if (str5cmp(m, 'R', 'e', 'f', 'e', 'r'))
                    {
                        state = sw_space_before_value;
                        httprequest->header_state = HS_REFERER;
                        break;
                    }
                    state = sw_space_after_value;
                    break;
                case 10: //User-Agent
                    if (str5cmp(m, 'U', 's', 'e', 'r', '-'))
                    {
                        state = sw_space_before_value;
                        httprequest->header_state = HS_USER_AGENT;
                        break;
                    }
                    state = sw_space_after_value;
                    break;
                case 14: //Content-Length
                     if (str5cmp(m, 'C', 'o', 'n', 't', 'e'))
                     {
                         state = sw_space_before_value;
                         httprequest->header_state = HS_CONTENT_LENGTH;
                         break;
                     }
                     state = sw_space_after_value;
                     break;
                default:
                    state = sw_space_after_value;
                    break;
                }

            }
            break;
        case sw_space_before_value:

            if (ch == ':')
            {
                break;
            }

            if (ch == ' ')
            {
                break;
            }

            m = p;
            state = sw_value;
            break;
        case sw_value:
            switch (ch)
            {
            case ' ':
                //state = sw_space_after_value;
                break;
            case CR :
                switch (httprequest->header_state)
                {
                case HS_HOST:
                    httprequest->host_start = m;
                    httprequest->host_end = p - 1;
                    break;
                case HS_USER_AGENT:
                    httprequest->ua_start = m;
                    httprequest->ua_end = p - 1;
                    break;
                case HS_REFERER:
                    httprequest->referer_start = m;
                    httprequest->referer_end = p - 1;
                    break;
                case HS_COOKIE:
                    httprequest->cookies_start = m;
                    httprequest->cookies_end = p - 1;
                    break;
                case HS_CONTENT_LENGTH:
                     httprequest->content_len_start = m;
                     httprequest->content_len_end = p - 1;
                     break;
                default:
                    return 22;
                }
                break;
            case LF :
                state = sw_head_start;
                break;
            case '\0':
                return 23;
            default:
                break;
            }
            break;

        case sw_space_after_value:
            switch (ch)
            {
            case ' ':
                break;
            case CR :
                break;
            case LF :
                state = sw_head_start;
                break;
            case '\0':
                return 24;
            default:
                break; //return 1;
            }
            break;
        case sw_almost_done:
            switch (ch)
            {
            case LF :
            	httprequest->post_start = p+1;
                goto request_done;
            case CR :
                break;
            default:
                return 25;
            }
            break;
        default:
            return 26;
        }
    }
    request_done:
    return 0;
}

/*
 * transfer eth packet to http packet
 */
int http_parser_level_1(int threadid, struct pfring_pkthdr * hdr, u_char *buffer, struct http_request_kinfo *http)
{
    http->host_start = http->host_end =  http->ua_start = http->ua_end
            = http->referer_start = http->referer_end = http->uri_start
                    = http->uri_end = http->cookies_start = http->cookies_end
                    = http->post_start = http->post_end
                            = http->content = NULL;
    http->http_type = 0;
    http->post_end = buffer + hdr->caplen;
    //init with have parser values
    http->sec = hdr->ts.tv_sec ;
    http->usec = hdr->ts.tv_usec;

    if(hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id != NO_TUNNEL_ID)
    {
        //printf("tunnel ==%ld\n", hdr->extended_hdr.parsed_pkt.tunnel.tunnel_id);
        if((hdr->extended_hdr.parsed_pkt.tunnel.tunneled_l4_dst_port != 80))
            return -66;


        http->sip = hdr->extended_hdr.parsed_pkt.tunnel.tunneled_ip_src.v4;
        http->dip = hdr->extended_hdr.parsed_pkt.tunnel.tunneled_ip_dst.v4;

	int tcp_header_len = hdr->extended_hdr.parsed_pkt.offset.payload_offset ;
        struct tcphdr * tcp = (struct tcphdr *)(&buffer[tcp_header_len
                +sizeof(struct compact_ip_hdr)]);

        int header_len = tcp_header_len+sizeof(struct compact_ip_hdr) + (tcp->doff) * 4;
        http->content = (u_char *) (buffer + header_len);
        http->len = hdr->caplen-header_len;

    }
    else
    {
        if (hdr->extended_hdr.parsed_pkt.l4_dst_port != 80)
            return -6;


        http->sip = hdr->extended_hdr.parsed_pkt.ipv4_src;
        http->dip = hdr->extended_hdr.parsed_pkt.ipv4_dst;

        int header_len = hdr->extended_hdr.parsed_pkt.offset.payload_offset;
        http->content = (u_char *) (buffer + header_len);
        http->len = hdr->caplen-header_len;
    }
    //init other values
    return 0;
}
