//============================================================================
// Name        : arduino.cpp
// Author      : eiithel
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Uart.h"
#include <mosquitto.h>
#include <stdio.h>
#include <stdint.h>
#include "Client.h"




using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	Client _client;
	_client.init_connection();

	Uart arduino;
	arduino.init();
	arduino.open("/dev/ttyACM0");

	char* buf = "1 on\n";
	int taille = strlen(buf);
	bool res;

	if(mosquitto_loop_start(_client._mosq) != MOSQ_ERR_SUCCESS) {
		std::cout << "Failed mosquitto init " << _client._mosq << std::endl;
		return 0;
	}

	while(1){
		//if(_client.circleDetected){
		res =_client.is_new_circle();
		if(_client.circleDetected){

			arduino.write(buf,taille);
			_client.circleDetected = false;
		}
	}

	printf("exit\n");
	arduino.close();

	return 0;
}

