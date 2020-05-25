#ifndef QRCODECOMM_H
#define QRCODECOMM_H

#include "serialPort.h"
#include <QObject>
#include <QString>

/**
 * @class QRcodeComm
 * @brief The QRcodeComm class, derived both from <b>QObject</b> and
 * <b>serialPort</b> to interface between them.
 */
class QRcodeComm: public QObject, public serialPort {
Q_OBJECT
private:

	/**
	 * @brief Implements the callback method that receives the answer from
	 * the serial port input buffer.
	 */
	void messageReceived(size_t bytes);

	/**
	 * @brief QString instance where the character buffer from the
	 * serial input line will be inserted to be signaled out to the GUI.
	 */
	QByteArray message;

public:
signals:

	/**
	 * @brief signalMessageReceived Signals the new character buffer from
	 * the serial input port has a new message.
	 * @param text The <b>QString</b> instance into which the serial input
	 *				buffer was transfered to.
	 */
	void signalMessageReceived(QByteArray * text);

};

#endif // QRCODECOMM_H
