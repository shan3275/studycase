#ifndef __YD_CAPTURE_H__
#define __YD_CAPTURE_H__
#include "pub_head.h"

void* pkt_cap();

void releaseNetCapture();


void capture_init(int nthreads);
void capture_destory(void);

#endif