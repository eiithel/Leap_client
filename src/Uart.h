/*
 * Uart.h
 *
 *  Created on: 11 avr. 2016
 *      Author: ethel
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include <stdint.h>

#include "unistd.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <stdlib.h>
#include "stdarg.h"
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <string>
#include <iostream>


typedef int FD;

class Uart {
public:
	Uart();
	virtual ~Uart();
	void open(const std::string& device);
	void init();
	void close();

	uint read(void* buf, uint size);
	uint write(const void* buf, uint size);

	FD getFD() {return mfd;}
	void ordreServo();


	FD mfd;
};

#endif /* SRC_UART_H_ */
