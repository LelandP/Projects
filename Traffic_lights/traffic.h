#ifndef PROD_H_
#define PROD_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <sys/dispatch.h>

#define ATTACH_POINT "TrafficControl"
#define NS_G 1
#define NS_Y 2
#define ALL_RED 0
#define EW_G 3
#define EW_Y 4
#define GREEN_DELAY 9000
#define YELOW_DELAY 3000
#define RED_DELAY 3000
#define RUNTIME 1000
#define SCREEN_LINES 6
#define BLINK_DELAY 1000
#define SERVER_REFRESH 5000

typedef struct{
	int running;
	int lightflag;
	int delay_ns_y;
	int delay_ns_g;
	int delay_ew_y;
	int delay_ew_g;
	int RED;
}traffic_msg;

#endif

