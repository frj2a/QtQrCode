/** @file serialPortDriver.cpp
 *
 * @brief Implementation of serialPortDriver methods
 *
 * This interface should be used to interact with any device or I/O
 * pin in order to abstract the access an improve the readability
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2019 Easytech Tecnologia LTDA.
 * Todos os direitos são reservados.
 */

/********************************************************************************************
 * INCLUDES
 ********************************************************************************************/

#include "serialPortDriver.h"
#include "serialPort.h"


/********************************************************************************************
 * LOCAL DEFINES
 ********************************************************************************************/

#define TERMINATOR	'\r'

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


static ThreadParams readThreadParams ;


/********************************************************************************************
 * IMPLEMENTATION
 ********************************************************************************************/



serialPortDriver::serialPortDriver()
{
	fd_ = 0;
	threadParams = &readThreadParams;
	portOpened_ = false;
}

serialPortDriver::~serialPortDriver()
{
	if (portOpened_)
	{
		closePort();
	}
}

bool serialPortDriver::openPort(std::string &filepath, int32_t &baudRate, std::string &mode, cc_t &timeout, serialPort * receiverObject)
{
	threadParams->receiver = receiverObject;

	if ( !filepath.empty() && (0 != baudRate) && !mode.empty() )
	{
		filepath_ = filepath;
		timeout_ = timeout;
		mode_ = mode;
		baud_ = B1152000;
		portOpened_ = false;
		switch(baudRate)
		{
			case 50:
				baud_ = B50;
			break;
			case 75:
				baud_ = B75;
			break;
			case 110:
				baud_ = B110;
			break;
			case 134:
				baud_ = B134;
			break;
			case 150:
				baud_ = B150;
			break;
			case 200:
				baud_ = B200;
			break;
			case 300:
				baud_ = B300;
			break;
			case 600:
				baud_ = B600;
			break;
			case 1200:
				baud_ = B1200;
			break;
			case 1800:
				baud_ = B1800;
			break;
			case 2400:
				baud_ = B2400;
			break;
			case 4800:
				baud_ = B4800;
			break;
			case 9600:
				baud_ = B9600;
			break;
			case 19200:
				baud_ = B19200;
			break;
			case 38400:
				baud_ = B38400;
			break;
			case 57600:
				baud_ = B57600;
			break;
			case 115200:
				baud_ = B115200;
			break;
			case 230400:
				baud_ = B230400;
			break;
			case 460800:
				baud_ = B460800;
			break;
			case 500000:
				baud_ = B500000;
			break;
			case 576000:
				baud_ = B576000;
			break;
			case 921600:
				baud_ = B921600;
			break;
			case 1000000:
				baud_ = B1000000;
			break;
			case 1152000:
				baud_ = B1152000;
			break;
			case 1500000:
				baud_ = B1500000;
			break;
			case 2000000:
				baud_ = B2000000;
			break;
			case 2500000:
				baud_ = B2500000;
			break;
			case 3000000:
				baud_ = B3000000;
			break;
			case 3500000:
				baud_ = B3500000;
			break;
			case 4000000:
				baud_ = B4000000;
			break;
			default:
				baud_ = B9600;
		}
		return (openPort());
	}
	else {
		return (false);
	}
}

bool serialPortDriver::openPort()
{
	int status;
	bool success = true;
	if (!portOpened_)
	{
		bool commonPort = ( std::string::npos != filepath_.find("tty") );

		tcflag_t cbits = CS8;
		tcflag_t cpar = 0;
		tcflag_t ipar = IGNPAR;
		tcflag_t isfl = 0;
		tcflag_t bstop = 0;	// 0 means 1 stop bit;
		if (4 != mode_.size())
		{
			success = false;
		}
		else
		{
			switch(mode_[0])
			{
				case '8':
					cbits = CS8;
				break;
				case '7':
					cbits = CS7;
				break;
				case '6':
					cbits = CS6;
				break;
				case '5':
					cbits = CS5;
				break;
				default:
					success = false;
			}
			switch(mode_[1])
			{
				case 'N':
				case 'n':
					cpar = 0;
					ipar = IGNPAR;
				break;
				case 'E':
				case 'e':
					cpar = PARENB;
					ipar = INPCK;
				break;
				case 'O':
				case 'o':
					cpar =(PARENB | PARODD);
					ipar = INPCK;
				break;
				default:
					success = false;
			}
			switch(mode_[2])
			{
				case '1':
					bstop = 0;
				break;
				case '2':
					bstop = CSTOPB;
				break;
				default:
					success = false;
			}
			if (success)
			{
				if (mode_[3]=='S')
				{
					isfl = IXON | IXOFF | IXANY;
				}
				else
				{
					isfl = 0;
				}
				if (commonPort)		// common ports;
				{
					fd_ = open(filepath_.c_str(), O_RDWR | O_NOCTTY | O_ASYNC);
				}
				else		// pts terminals
				{
					fd_ = open(filepath_.c_str(), O_RDWR            | O_ASYNC);
				}
				if (fd_ == -1)
				{
					perror("unable to open comport ");
					success = false;
				}
				else
				{
					if (commonPort)		// common ports;
					{
						int error = tcgetattr(fd_, &old_port_settings);
						if (error == -1)
						{
							close(fd_);
							success = false;
						}
						else
						{
							memset(&new_port_settings, 0, sizeof(new_port_settings));

							// new_port_settings.c_cflag &= ~PARENB;
							// new_port_settings.c_cflag &= ~PARODD;
							// new_port_settings.c_cflag &= ~CSTOPB;
							// new_port_settings.c_cflag &= ~CSIZE;
							new_port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;

							// new_port_settings.c_iflag &= ~(IXON | IXOFF | IXANY);
							new_port_settings.c_iflag = ipar | isfl;

							// new_port_settings.c_oflag &= ~OPOST;
							new_port_settings.c_oflag = 0;		// raw output;

							// new_port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
							new_port_settings.c_lflag = 0;

							new_port_settings.c_cc[VMIN] = 0;			// blocking read until 1 character arrives or 'read' times out;
							new_port_settings.c_cc[VTIME] = timeout_;	// VTIME in tenths of a second;
							cfsetispeed(&new_port_settings, baud_);
							cfsetospeed(&new_port_settings, baud_);
							error = tcsetattr(fd_, TCSANOW, &new_port_settings);
							if (error == -1)
							{
								close(fd_);
								success = false;
							}
							else
							{
								if (ioctl(fd_, TIOCMGET, &status)== -1)
								{
									perror("unable to get portstatus");
									success = false;
								}
								else
								{
									switch(mode_[3])
									{
										case 'N':
										case 'n':
											status &= ~TIOCM_DTR;
											status &= ~TIOCM_RTS;
										break;
										case 'S':
										case 's':
											status &= ~TIOCM_DTR;
											status &= ~TIOCM_RTS;
										break;
										case 'H':
										case 'h':
											status |= TIOCM_DTR;
											status |= TIOCM_RTS;
										break;
										default:
											success = false;
									}
									if (success)
									{
										if (ioctl(fd_, TIOCMSET, &status)== -1)
										{
											success = false;
										}
										portOpened_ = success;
									}
								}
							}
						}
					}
					else
					{
						portOpened_ = success;
					}
				}
			}
		}
		if (success)
		{
			startThread();
		}
	}
	return(success);
}

bool serialPortDriver::closePort()
{
	stopThread();
	portOpened_ = false;
	tcsetattr(fd_, TCSANOW, &old_port_settings);
	return (close(fd_));
}

void serialPortDriver::readPortThread(void* threadParams)
{
	ThreadParams * p = static_cast<ThreadParams*>(threadParams);
	p->running = true;

	ssize_t n = 0;
	uint16_t idx = 0;

	p->in_buf[idx] = 0;

	while( p->keepGoing )
	{
		p->messageReady = true;
		do
		{
			do
			{
				n = read(p->port_fd_, p->in_buf + idx, 1); 	// just one char at a time - or none;
			} while ( (n <= 0) && p->keepGoing );
			idx++;
			if ( IN_BUFFER_SIZE <= idx )
			{
				idx = 0;
			}
			p->in_buf[idx] = 0;
		}
		while( p->keepGoing && (TERMINATOR != p->in_buf[idx-1]) );

		if ( p->keepGoing )
		{
			p->receivingMessage = false;
			p->messageReady = true;
			*p->bytes_received = idx-1;
			p->receiver->messageReceived();
		}
		p->messageReady = false;
		n = 0;
		idx = 0;
		p->in_buf[idx] = 0;
	}
	p->running = false;
}

void  serialPortDriver::startThread()
{
	threadParams->port_fd_ = fd_;
	threadParams->in_buf = threadParams->receiver->in_buf;
	threadParams->bytes_received = &threadParams->receiver->bytes_received;
	threadParams->receivingMessage = false;
	threadParams->keepGoing = true;		// this is tested at the end of the thread function;
	threadParams->messageReady = false;
	threadParams->receivingMessage = false;
	threadParams->running = true;
	std::thread readPortThread_thread (readPortThread, threadParams);
	readPortThread_thread.detach();
}

void serialPortDriver::stopThread()
{
	if (threadParams->running)
	{
		threadParams->keepGoing = false;
		do
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
		}
		while (threadParams->running);
		close(fd_);
		portOpened_ = false;
	}
}

ssize_t serialPortDriver::writePort(std::string &message)
{
	return (write(this->fd_, message.c_str(), message.size()));
}
