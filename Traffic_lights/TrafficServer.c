#include "traffic.h"

int main (void){
	name_attach_t *attach;
	traffic_msg msg;
	int rcvid;
	int counter = 0;
	int count = 12;

	if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
		perror("Server side: ");
	    return EXIT_FAILURE;
	}

	while (1) {
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

		/* Error condition, exit */
	    if (rcvid == -1) {
	    	perror("MsgReceive ");
	    	break;
	    }

	    switch (msg.lightflag)
	    {
			case NS_G:
				printf("Client NS Light is now GREEN\n");
				break;
			case NS_Y:
				printf("Client NS Light is now YELLOW\n");
				break;
			case EW_G:
				printf("Client EW Light is now GREEN\n");
				break;
			case EW_Y:
				printf("Client EW Light is now YELLOW\n");
				break;
			case ALL_RED:
				printf("Client all Lights are now RED\n");
				break;
			default:
				break;
	    }

	    if (counter % 48 == 0 || count < 12){
	    	if (count == 12){
	    		count = 1;
	    		printf("Rush hour Start\n");
	    	}

	    	msg.delay_ns_g = 13000;
	    	msg.delay_ew_g = 7000;
	    	count++;
	    	if (count == 12){
				printf("Rush hour Stop\n");
	    	}
	    }else{

	    	msg.delay_ns_g = 9000;
	    	msg.delay_ew_g = 9000;

	    }
	    MsgReply(rcvid, EOK, &msg, sizeof(msg));
	    counter++;
	 }

	 /* Remove the name from the space */
	 name_detach(attach, 0);

	return EXIT_SUCCESS;
}
