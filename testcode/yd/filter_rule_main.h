#ifndef __FILTER_RULE_MAIN__
#define __FILTER_RULE_MAIN__

#include "init.h"

/*main filter loop*/
int http_cap_filter_main(const int thread_id, u_char *buffer, struct pfring_pkthdr* hdr);

#endif
