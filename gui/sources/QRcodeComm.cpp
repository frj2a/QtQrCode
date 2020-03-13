#include "QRcodeComm.h"

void QRcodeComm::messageReceived()
{
	message = QString(in_buf);
	emit signalMessageReceived(&message);
}
