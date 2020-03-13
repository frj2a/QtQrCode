/** @file serialPortDriver.hpp
 *
 * Communication port abstraction to transfer and receive messages
 * through UART RS232 serial port.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Easytech Tecnologia LTDA.
 * Todos os direitos são reservados.
 */

#ifndef SERIALPORTDRIVER_H_
#define SERIALPORTDRIVER_H_

/********************************************************************************************
 * INCLUDES
 ********************************************************************************************/

#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>

/********************************************************************************************
 * LOCAL DEFINES
 ********************************************************************************************/

#define IN_BUFFER_SIZE			2048

/********************************************************************************************
 * LOCAL TYPEDEFS
 ********************************************************************************************/

class serialPort;

/**
 * @struct ThreadParams
 * @brief The ThreadParams struct is used to exchange working members between
 * the static thread serial line polling method and the dynamic part of the
 * program.
 */
struct ThreadParams {
	char			* in_buf;			//!< Input buffer, _IN_BUFFER_SIZE_ sized.
	uint16_t		* bytes_received;	//!< number of received bytes in Input buffer.
	volatile bool	messageReady;		//!< Message received successfuly (true) or maximum trials reached (false);
	volatile bool	receivingMessage;	//!< Tells if the thread is receiving a characters but didn't get the terminator;
	volatile bool	keepGoing;			//!< Defines if the thread loops for ever or runs just once.
	volatile bool	running;			//!< Running state of the thread.
	int				port_fd_;			//!< Serial port ID in use.
	serialPort 		* receiver;			//!< External object's method that will work on the received message.
};

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
 * @class serialPortDriver
 * @brief The serialPortDriver class, that interfaces
 * directly to the serial port device from the Operating System.
 *
 * It uses the file path used by kernel to abstract
 * the communication medium between the User Space
 * and the device.
 */
class serialPortDriver {

public:

	explicit serialPortDriver();


	~serialPortDriver();

	/**
	 * @brief Open connection to an UART device
	 *
	 * @param filepath File path to UART access file.
	 * @param baudRate Bit rate speed.
	 * @param mode String built from 4 characters, like 8N1N, 8N1H, 7N2S: \
	 *				first character means number of bits for each packet (7 or 8), \
	 *				second character means parity (N, O or E),
	 *				third character is the number of stop bits (1 or 2) and
	 *				the last character is the flow control (N, H or S).
	 * @param timeout Time out for the the "unistd.h" read instruction in
	 *				tenths of a second: the <b>read</b> function normaly
	 *				blocks until a byte is received, so the timeout is here
	 *				to unlock the read function so that thread can check if
	 *				it is time to stop;
	 * @param receiverObject Object directly or derived from <b>serialPort</b>,
	 *				that contains the call-back method <b>messageReceived</b>.
	 *
	 * @return <b>true</b> if the device was opened properly.
	 */
	bool openPort(std::string &filepath, int32_t &baudRate, std::string &mode, cc_t &timeout, serialPort * receiverObject);

	/**
	 * @brief Open connection to a  UART device with an already known configuration.
	 *
	 * @return <b>true</b>  if the device was opened properly.
	 */
	bool openPort();

	/**
	 * @brief Closes the connection to the previously opened UART device.
	 *
	 * @return <b>true</b>  if the device was closed properly.
	 */
	bool closePort();

	/**
	 * @brief Writes data to the UART device.
	 *
	 * It sends the message/command stored in the input string.
	 *
	 * @param message Container where the message/command to be sent to the device is stored.
	 *
	 * @return the size of the written string, if the message/command was written properly, otherwise -1.
	 */
	ssize_t writePort(std::string &message);

	/**
	 * @brief Thread to keep reading data from the UART device.
	 *
	 * It uses a reference to a structure where the needed input parameters
	 * and output results are put.
	 *
	 * @param threadParams Container of the  needed input parameters
	 *						and output results are put.
	 */
	static void readPortThread  (void* threadParams);


	/**
	 * @brief Check if the port is already opened.
	 * @return <b>true</b> if the port is opened.
	 */
	bool isOpened() {
		return (portOpened_);
	}


private:

	/**
	 * @brief Starts the receiving line polling thread.
	 */
	void startThread();


	/**
	 * @brief Stops the receiving line polling thread.
	 */
	void stopThread();


/*
	ssize_t readChar(int, char *);


	// void readCharComPort(int, char *, ssize_t * size);


	bool sendByte(int, char);


	void cputs(int, char *);


	void closeComport(int);
*/


private:


	/**
	 * \brief	IO File decriptor used for the serial port.
	 */
	int				fd_;

	/**
	 * \brief	IO access timeout in msec.
	 */
	cc_t			timeout_;	// in tenths of second - ds (or decisecond)

	/**
	 * \brief	Device file path.
	 */
	std::string		filepath_;

	/**
	 * \brief	Desired speed (baud rate).
	 */
	speed_t			baud_;

	/**
	 * \brief	Desired operation mode (as '8N1H').
	 */
	std::string		mode_;

	/**
	 * \brief	Flag to check if the port was already opened.
	 */
	bool			portOpened_;


	/**
	 * \brief	Mutable parameters to and from the serial receiving thread.
	 */
	ThreadParams *	threadParams;


	/**
	 * \brief	New port settings, set up from user space parameters.
	 */
	struct termios	new_port_settings;


	/**
	 * \brief	Original port settings, read before changing anything, restored prior to close the port.
	 */
	struct termios	old_port_settings;

};


#endif // SERIALPORTDRIVER_H_
