/*
 * Uart.cpp
 *
 *  Created on: 11 avr. 2016
 *      Author: ethel
 */

#include "Uart.h"

Uart::Uart():mfd(0) {
	// TODO Auto-generated constructor stub

}

Uart::~Uart() {
	close();
}

void Uart::open(const std::string& device){
	mfd = ::open(device.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (mfd < 0) {
		printf("ERROR: %s\n", strerror(errno));
	}
}


void Uart::init(){

	struct termios   options;    // Terminal options
	int rc;         			// Return value
	// Get the current options for the port
	if((rc = tcgetattr(mfd, &options)) < 0){
		fprintf(stderr, "failed to get attr: %d, %s\n", mfd, strerror(errno));
	}

	cfmakeraw(&options);
	options.c_cflag |= (CLOCAL | CREAD);   // Enable the receiver and set local mode
	options.c_cflag &= ~CSTOPB;            // 1 stop bit
	options.c_cflag &= ~CRTSCTS;           // Disable hardware flow control
	options.c_cc[VMIN]  = 1;
	options.c_cc[VTIME] = 2;
	options.c_cflag &= ISIG;//Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals

	// Set the new attributes
	if((rc = tcsetattr(mfd, TCSANOW, &options)) < 0){
		fprintf(stderr, "failed to set attr: %d, %s\n", mfd, strerror(errno));
	}
}

void Uart::close(){
	if(::close(mfd) != 0) {
		printf("ERROR: %s\n", strerror(errno));
	}
	mfd = 0;
}


uint Uart::read(void* buf, uint size){

	uint res;
	if((res = ::read(mfd, buf, size)) < 0) {
		printf("ERROR: %s\n", strerror(errno));
	}
	return res;
}

uint Uart::write(const void* buf, uint size){
	printf("\nentree dans la fonction write\n");
	uint res;
	if((res = ::write(mfd, buf, size)) < 0) {
		printf("ERROR: %s\n", strerror(errno));
	}
	return res;

}

void Uart::ordreServo(){

	char* buf = "1 on\n";
	int taille = strlen(buf);
	this->write(buf,taille);

}

