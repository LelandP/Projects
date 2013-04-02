#include "traffic.h"

void* lightSystem();
void* controller();
void* connect();
/*He are are the globals
 * we are using globals
 * to keep the software
 * as fast and to not need
 * to connect to shared memory*/

/*Ensures it has a connection to server*/
int coid = 0;
/*To exit out of programme*/
int running = 1;
traffic_msg msg;
/*We use a mutex in this case so that nothing
 * can touch the mesage struct while it is being
 * sent, due to the fact that some variables
 * are changed in the server*/
pthread_mutex_t lock;

int main(int argc, char *argv[]) {
	pthread_attr_t lights, connection;
	int lights_id, connection_id;

	msg.delay_ew_g = msg.delay_ns_g = GREEN_DELAY;
	msg.delay_ew_y = msg.delay_ns_y = YELOW_DELAY;
	msg.RED = RED_DELAY;

	pthread_attr_init(&lights);
	pthread_attr_init(&connection);

	pthread_attr_setdetachstate(&lights, PTHREAD_CREATE_DETACHED );
	pthread_attr_setdetachstate(&connection, PTHREAD_CREATE_DETACHED );

	connection_id = pthread_create(NULL, &connection, &controller, NULL );
	lights_id = pthread_create(NULL, &lights, &lightSystem, NULL );
	sleep(RUNTIME);

	pthread_join(lights_id, NULL);
	pthread_join(connection_id, NULL);

	return EXIT_SUCCESS;
}

void* lightSystem() {
	int i = 0;
	int odd = 0;
	while (running) {
		if (coid > 0) {
			/*If lights have proper connection to server*/

			/** HANDLE NORTH SOUTH GREEN LIGHT **/
			//print blank lines to simulate live exchange
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			//prints the lights on the screen
			printf("          G=X\n");
			printf("          Y=o\n");
			printf("          R=o\n");
			printf("G=o                   G=o\n");
			printf("Y=o                   Y=o\n");
			printf("R=X                   R=X\n");
			printf("          G=X\n");
			printf("          Y=o\n");
			printf("          R=o\n");
			/*locks this section because we are handeling a
			/*very important struct*/
			pthread_mutex_lock(&lock);
			//here we change the light flag to NS_g
			msg.lightflag = NS_G;
			//we send the struct to the server for processing
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			//unlock the thread
			pthread_mutex_unlock(&lock);
			//use the delay set by the server to keep the light on.
			delay(msg.delay_ns_g);

			/** HANDLE NORTH SOUTH YELLOW LIGHT **/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("          G=o\n");
			printf("          Y=X\n");
			printf("          R=o\n");
			printf("G=o                   G=o\n");
			printf("Y=o                   Y=o\n");
			printf("R=X                   R=X\n");
			printf("          G=o\n");
			printf("          Y=X\n");
			printf("          R=o\n");
			pthread_mutex_lock(&lock);
			msg.lightflag = NS_Y;
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			pthread_mutex_unlock(&lock);
			delay(msg.delay_ns_y);

			/** HANDLE ALL LIGHTS ARE RED **/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			printf("G=o                   G=o\n");
			printf("Y=o                   Y=o\n");
			printf("R=X                   R=X\n");
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			pthread_mutex_lock(&lock);
			msg.lightflag = ALL_RED;
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			pthread_mutex_unlock(&lock);
			delay(msg.RED);

			/** HANDLE EAST WEST LIGHTS ARE GREEN **/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			printf("G=X                   G=X\n");
			printf("Y=o                   Y=o\n");
			printf("R=o                   R=o\n");
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			pthread_mutex_lock(&lock);
			msg.lightflag = EW_G;
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			pthread_mutex_unlock(&lock);
			delay(msg.delay_ew_g);

			/** HANDLE EW LIGHTS ARE YELLOW **/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			printf("G=o                   G=o\n");
			printf("Y=X                   Y=X\n");
			printf("R=o                   R=o\n");
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			pthread_mutex_lock(&lock);
			msg.lightflag = EW_Y;
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			pthread_mutex_unlock(&lock);
			delay(msg.delay_ew_y);

			/** HANDLE ALL LIGHTS RED **/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			printf("G=o                   G=o\n");
			printf("Y=o                   Y=o\n");
			printf("R=X                   R=X\n");
			printf("          G=o\n");
			printf("          Y=o\n");
			printf("          R=X\n");
			pthread_mutex_lock(&lock);
			msg.lightflag = ALL_RED;
			if (MsgSend(coid, &msg, sizeof(msg), &msg, sizeof(msg)) == -1) {
				perror(
						"Client :: Could not send message :: Server turned on? :: ");
				coid = -1;
				continue;
			}
			pthread_mutex_unlock(&lock);
			delay(msg.RED);

		} else {
			/*If lights dont have connection to server they blink*/
			for (i = 0; i < SCREEN_LINES; i++) {
				printf("\n");
			}
			printf("Light system is not connected to server!!\n");

			if (odd) {
				printf("          G=o\n");
				printf("          Y=o\n");
				printf("          R=o\n");
				printf("G=o                   G=o\n");
				printf("Y=o                   Y=o\n");
				printf("R=o                   R=o\n");
				printf("          G=o\n");
				printf("          Y=o\n");
				printf("          R=o\n");
				odd--;
			} else {
				printf("          G=o\n");
				printf("          Y=o\n");
				printf("          R=X\n");
				printf("G=o                   G=o\n");
				printf("Y=o                   Y=o\n");
				printf("R=X                   R=X\n");
				printf("          G=o\n");
				printf("          Y=o\n");
				printf("          R=X\n");
				odd++;
			}
		}
		delay(BLINK_DELAY);
	}
	return 0;
}

/*This thread checks for server connection every 5000 miliseconds*/
void* controller() {
	/*Starts and alarm to create a 10 second delay
	 * to give the server time to start up.*/
	alarm(10);
	signal(SIGALRM, *connect);
	return 0;
}
void* connect() {
	/*Loops threw infinitly to refresh the server conecction*/
	while (running) {
		if ((coid = name_open(ATTACH_POINT, 0)) == NULL) {
			perror("Client side: Server turned on? : ");
		}
		delay(SERVER_REFRESH);
	}
	return 0;
}
