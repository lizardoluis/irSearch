/*
 * socket.h
 *
 *  Created on: Apr 4, 2014
 *      Author: lizardo
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_SIZE (1000)
#define PORT (10001)

#define SOCKET_OK "220\r\n"
#define END_MSG "320\r\n"

void open_server(int *sd);

void accept_socket(int *sd, int *sd_current);

void recv_socket(int sd, char *msg);

void send_socket(int sd, char *msg);

#endif /* SOCKET_H_ */
