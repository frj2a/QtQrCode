#include "QRcodeComm.h"

void QRcodeComm::messageReceived(size_t bytes) {
	message = QByteArray(in_buf, bytes);
	emit signalMessageReceived(&message);
}
