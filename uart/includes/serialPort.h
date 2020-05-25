/** @file serialPort.h
 *
 * Communication abstraction to receive messages
 * through UART port.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2020 Easytech Tecnologia LTDA.
 * Todos os direitos são reservados.
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

/********************************************************************************************
 * INCLUDES
 ********************************************************************************************/

#include "serialPortDriver.h"

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
* DECLARATIONS
 ********************************************************************************************/


/**
 * @class serialPort
 * @brief The serialPort class, as seen from a higher level of abstraction.
 *
 * This class properly initializes and closes the serial device.
 * It <b>must</b> be derived from <b>serialPort</b> to implement
 * the method <b>serialPort::messageReceived</b> that is a call-back to
 * anounce the received buffer when the <b>serialPortDriver</b> input
 * polling thread receives a delimiter character.
 */
class serialPort {
protected:

	serialPort();

	virtual ~serialPort();

public:

	/**
	 * @brief Receives the answer from serial port input buffer.
	 */
	virtual void messageReceived(size_t bytes) = 0;


	/**
	 * @brief Opens the specified serial port using the configurations given.
	 *
	 * @param port device name to be handled;
	 * @param baudRate bit rate for transfers;
	 * @param mode string like 8N1N, 8N1H, 7N2S: \
	 *				first character means number of bits for each packet (7 or 8), \
	 *				second character means parity (N, O or E),
	 *				third character is the number of stop bits (1 or 2) and
	 *				the last character is the flow control (N, H or S);
	 * @param timeout in tenths of a second: the "unistd.h" read instruction normaly blocks
	 *				until a byte is received, so the timeout is here to unlock the read
	 *				instruction so that thread can check if it is time to stop;
	 * @return <b>true</b> if the device was opened properly.
	 */
	bool openSerialPort(std::string &port, int32_t &baudRate, std::string &mode, cc_t & timeout_tr, bool waitTerminator, unsigned char terminator);


	/**
	 * @brief Reopens the serial port with the same configurations used in a previous opening.
	 *
	 * @return <b>true</b> if the device was opened properly.
	 */
	bool reopenSerialPort(bool waitTerminator, unsigned char terminator);


	/**
	 * @brief Closes the serial port.
	 *
	 * @return <b>true</b> if the device was closed properly.
	 */
	bool closeSerialPort();


	/**
	 * @brief Allows checking if the serial port is opened.
	 *
	 * @return <b>true</b> if the device is opened properly.
	 */
	bool isSerialPortOpened() {
		return sPort->isOpened();
	}


	bool write(char * buffer, int len);


protected:

	ThreadParams				* threadParams;			//!< Mutable parameters to and from the serial receiving thread.
	serialPortDriver			* sPort;				//!< The serial port handler.
	char						in_buf[IN_BUFFER_SIZE];	//!< Input buffer, used in the receiving thread.
	uint16_t					bytes_received;			//!< number of bytes received in each message from the reader.


friend class serialPortDriver;
};

#endif // SERIALPORT_H_
