/*
 * socket.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: lizardo
 */

#include "socket.h"

void open_server(int *sd) {
	struct sockaddr_in sin;

	/* get an internet domain socket */
	if ((*sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* complete the socket structure */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	/* bind the socket to the port number */
	if (bind(*sd, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("bind");
		exit(1);
	}

	/* show that we are willing to listen */
	if (listen(*sd, 5) == -1) {
		perror("listen");
		exit(1);
	}
}

void accept_socket(int *sd, int *sd_current) {
	struct sockaddr_in pin;
	socklen_t addrlen = sizeof(pin);

	/* wait for a client talk to us */
	if ((*sd_current = accept(*sd, (struct sockaddr *) &pin, &addrlen)) == -1) {
		perror("accept");
		exit(1);
	}
}

void recv_socket(int sd, char *msg) {

	memset(msg, 0, MSG_SIZE);  //clear the variable
	if (recv(sd, msg, MSG_SIZE, 0) == -1) {
		perror("recv");
		exit(1);
	}
	//printf("r - %s", msg);
}

void send_socket(int sd, char *msg) {

	if (send(sd, msg, strlen(msg), 0) == -1) {
		perror("send");
		exit(1);
	}
	//printf("s - %s", msg);
}
