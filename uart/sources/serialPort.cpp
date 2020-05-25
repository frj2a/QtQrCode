/** @file serialPort.cpp
 *
 * @brief Abstraction of serialPortDriver methods.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Easytech Tecnologia LTDA.
 * Todos os direitos são reservados.
 */

/********************************************************************************************
 * INCLUDES
 ********************************************************************************************/

#include "serialPort.h"


/********************************************************************************************
 * LOCAL DEFINES
 ********************************************************************************************/


/********************************************************************************************
 * LOCAL TYPEDEFS
 ********************************************************************************************/


/********************************************************************************************
 * LOCAL FUNCTIONS
 ********************************************************************************************/


/********************************************************************************************
 * LOCAL VARIABLES
 ********************************************************************************************/


/********************************************************************************************
 * GLOBAL VARIABLES
 ********************************************************************************************/


/********************************************************************************************
 * IMPLEMENTATION
 ********************************************************************************************/

serialPort::serialPort() {
	memset(in_buf, 0, IN_BUFFER_SIZE);
	bytes_received = 0;
	threadParams = nullptr;
	sPort = new serialPortDriver();
}

serialPort::~serialPort() {
	if (sPort->isOpened()) {
		sPort->closePort();
	}
	delete sPort;
	sPort = nullptr;
}

bool serialPort::openSerialPort(std::string &port, int32_t &baudRate, std::string &mode, cc_t & timeout_tr, bool waitTerminator, unsigned char terminator) {
	return (sPort->openPort(port, baudRate, mode, timeout_tr, this, waitTerminator, terminator));
}

bool serialPort::reopenSerialPort(bool waitTerminator, unsigned char terminator) {
	bool returnResult = false;
	if (sPort != nullptr) {
		returnResult = (sPort->openPort(waitTerminator, terminator));
	}
	return (returnResult);
}

bool serialPort::closeSerialPort() {
	bool returnResult = false;
	if (sPort != nullptr) {
		returnResult = (sPort->closePort());
	}
	return (returnResult);
}

bool serialPort::write(char * buffer, int len) {
	return (sPort->writePort(buffer, len) == len);
}
