/*
 * Client.h
 *
 *  Created on: 9 avr. 2016
 *      Author: ethel
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <mosquitto.h>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include "Uart.h"


class Client {
public:
	Client();
	virtual ~Client();

	char _id[30];
	char *_host;
	int _port;
	int _keepalive;
	bool _clean_session;
	struct mosquitto *_mosq;
	bool circleDetected;


	int init_connection();
	bool send_message(const  char * message, const char *topic);
	int subscribe(const char *sub);
	int handleCircle();
	bool is_new_circle();



private:


};

#endif /* CLIENT_H_ */
