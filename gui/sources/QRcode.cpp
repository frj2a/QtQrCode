#include "QRcode.h"
#include <QPair>
#include <iterator>

#define BUFFER_SIZE	(9)

#define ALL_CONFIG_SIZE (237)

static const std::string base64_chars =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
										"abcdefghijklmnopqrstuvwxyz"
										"0123456789+/";

QRcode::QRcode(QWidget *parent) : QWidget(parent) {
	setupUi(this);
	serialPort = new QRcodeComm();
	waitTerminator = true;
	terminator = '\r';
	connect(serialPort, &QRcodeComm::signalMessageReceived, this, &QRcode::slotMessageReceived);
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
	alreadyOpened = false;

	unsigned char teste[6][BUFFER_SIZE] ={
		{0x7E, 0x00, 0x07, 0x01, 0x00, 0x00, 0xE5, 0x00, 0x00},	//<! read all zone bit
		{0x7E, 0x00, 0x07, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00},	//<! read zone bit from 0x00 to 0x0f
		{0x7E, 0x00, 0x07, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00},	//<! read zone bit from 0x10 to 0x1f
		{0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0xC2, 0x00, 0x00},	//<! set zone bit 0, bits 3-2 : turn LED light off
		{0x7E, 0x00, 0x08, 0x01, 0x00, 0x00, 0xC6, 0x00, 0x00},	//<! set zone bit 0, bits 3-2 : turn LED light on
		{0x7E, 0x00, 0x09, 0x01, 0x00, 0x00, 0x00, 0xDE, 0xC8}	//<! save zone bit to eeprom
	};
	unsigned char config[ALL_CONFIG_SIZE] = {
		0x7E, 0x00, 0x08, 0xE5, 0x00, 0x00,					// header, type, length, start address
		0xc2, 0x00, 0x00, 0x00, 0x00, 0x05, 0x1e, 0x01,		// data
		0x2c, 0x00, 0x64, 0x3c, 0x01, 0xa0, 0x1c, 0x64,		// data
		0x30, 0x00, 0x80, 0xb2, 0x06, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x39, 0x01, 0x05, 0x14, 0x08, 0x08,		// data
		0x08, 0x08, 0x08, 0x04, 0x04, 0x20, 0x00, 0x04,		// data
		0x20, 0x04, 0x04, 0x20, 0x00, 0x04, 0x20, 0x01,		// data
		0x00, 0x04, 0x20, 0x00, 0x04, 0x20, 0x00, 0x04,		// data
		0x20, 0x00, 0x04, 0x20, 0x04, 0x04, 0x20, 0x00,		// data
		0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x00, 0x14,		// data
		0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x64, 0x64, 0x63, 0x63, 0x63, 0x6a, 0x62,		// data
		0x69, 0x61, 0x65, 0x44, 0x76, 0x48, 0x6d, 0x52,		// data
		0x52, 0x52, 0x51, 0x75, 0x72, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// data
		0x00, 0x00, 0x55, 0x01, 0x64, 0x00, 0x00, 0x00,		// data
		0x02, 0x64, 0x7f, 0x12, 0x01,						// data
		0xA6, 0x79											// crc space
	};
	union {
		char complete[BUFFER_SIZE];
		struct __attribute__((packed)){
			uint8_t header[2];
			uint8_t type[1];
			uint8_t length[1];
			uint8_t start_address[2];
			uint8_t data[1];
			union {
				uint16_t integer;
				uint8_t bytes[2];
			} crc;
		} parts;
	} buffer_tests;

	memcpy(buffer_tests.complete,teste[0],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("01 Read all Zone Bit", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	memcpy(buffer_tests.complete,teste[1],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("02 Read Zone Bit from 0x00 to 0x0F", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	memcpy(buffer_tests.complete,teste[2],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("03 Read Zone Bit from 0x10 to 0x1F", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	memcpy(buffer_tests.complete,teste[3],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("04 Disable white LED light", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	memcpy(buffer_tests.complete,teste[4],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("05 Enable white LED light", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	memcpy(buffer_tests.complete,teste[5],BUFFER_SIZE);
	buffer_tests.parts.crc.integer = crc_cal_by_bit(buffer_tests.parts.type, BUFFER_SIZE-4);
	messages.insert("06 Save zone bit to eeprom", QByteArray(static_cast<char *>(buffer_tests.complete), BUFFER_SIZE));

	/*
		0x7E, 0x00, 0x08, 0xE5, 0x00, 0x00,					// header, type, length, start address
	 */
	union {
		char complete[ALL_CONFIG_SIZE];
		struct __attribute__((packed)){
			uint8_t header[2];
			uint8_t type[1];
			uint8_t length[1];
			uint8_t start_address[2];
			uint8_t data[ALL_CONFIG_SIZE-8];
			union {
				uint16_t integer;
				uint8_t bytes[2];
			} crc;
		} parts;
	} buffer_config;

	memcpy(buffer_config.complete, config, ALL_CONFIG_SIZE);
	buffer_config.parts.crc.integer = crc_cal_by_bit(buffer_config.parts.type, ALL_CONFIG_SIZE-4);
	messages.insert("07 Write all standard configuration", QByteArray(static_cast<char *>(buffer_config.complete), ALL_CONFIG_SIZE));

	for (QMap<QString,QByteArray>::iterator i = messages.begin(); i != messages.end(); i++) {
		cbMessages->addItem(i.key());
	}
	pbSend->setEnabled(false);
}

QRcode::~QRcode() {
	disconnect(serialPort, &QRcodeComm::signalMessageReceived, this, &QRcode::slotMessageReceived);
}

void QRcode::changeEvent(QEvent *e) {
	QWidget::changeEvent(e);
	switch (e->type( ) ) 	{
		case QEvent::LanguageChange:
			retranslateUi(this);
		break;
		default:
		break;
	}
}

void QRcode::on_pbOpenClose_clicked() {
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
	if (serialPort->isSerialPortOpened( ) ) {
		serialPort->closeSerialPort();
		leDevice->setEnabled(true);
		cbBaud->setEnabled(true);
		cbBits->setEnabled(true);
		cbParity->setEnabled(true);
		cbStopBits->setEnabled(true);
		cbFlow->setEnabled(true);
		leTimeOutShort->setEnabled(true);
		pbOpenClose->setText(tr("&Open" ) );
		pbSend->setEnabled(false);
	} else {
		if (alreadyOpened) {
			if (rbHex->isChecked()) {
				serialPort->reopenSerialPort(false, terminator);
			} else {
				serialPort->reopenSerialPort(true, terminator);
			}
		} else {
			unsigned int timeOutShort = leTimeOutShort->text().toUInt();
			if (timeOutShort > 255) {
				timeOutShort = 255;
			}
			std::string dev = leDevice->text().toStdString();
			int32_t baud = cbBaud->currentText().toInt();
			std::string smode = mode.toStdString();
			cc_t tout_tr = cc_t(timeOutShort);
			if (rbHex->isChecked()) {
				serialPort->openSerialPort(dev, baud, smode, tout_tr, false, terminator);
			} else {
				serialPort->openSerialPort(dev, baud, smode, tout_tr, true, terminator);
			}
		}
		if (serialPort->isSerialPortOpened( ) ) {
			leDevice->setEnabled(false);
			cbBaud->setEnabled(false);
			cbBits->setEnabled(false);
			cbParity->setEnabled(false);
			cbStopBits->setEnabled(false);
			cbFlow->setEnabled(false);
			leTimeOutShort->setEnabled(false);
			pbOpenClose->setText(tr("&Close" ) );
			pbSend->setEnabled(true);
		}
	}
}

void QRcode::on_pbClear_clicked() {
	pteMessages->clear();
}

void QRcode::on_pbSend_clicked() {
	QByteArray arry = messages[cbMessages->itemText(cbMessages->currentIndex())];
	pteMessages->appendPlainText(" ");
	if (serialPort->write(arry.data(), arry.length())) {
		pteMessages->appendPlainText(QString("> %1").arg(QString(arry.toHex(' '))));
	} else {
		pteMessages->appendPlainText("Error sending message.");
	}
}

void QRcode::on_leDevice_editingFinished() {
	alreadyOpened = false;
}

void QRcode::on_cbBaud_currentIndexChanged(QString) {
	alreadyOpened = false;
}

void QRcode::on_cbBits_currentIndexChanged(QString text) {
	alreadyOpened = false;
	mode = text + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::on_cbParity_currentIndexChanged(QString text) {
	alreadyOpened = false;
	mode = cbBits->currentText() + text + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::on_cbStopBits_currentIndexChanged(QString text) {
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + text + cbFlow->currentText();
}

void QRcode::on_cbFlow_currentIndexChanged(QString text) {
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + text;
}

void QRcode::on_leTimeOut_editingFinished() {
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::on_rbAscii_clicked() {
	serialPort->closeSerialPort();
	serialPort->reopenSerialPort(true, terminator);
}

void QRcode::on_rbBase64_clicked() {
	serialPort->closeSerialPort();
	serialPort->reopenSerialPort(true, terminator);
}

void QRcode::on_rbHex_clicked() {
	serialPort->closeSerialPort();
	serialPort->reopenSerialPort(false, terminator);
}

void QRcode::slotMessageReceived(QByteArray * message) {
	msg.append(*message);
	if (rbBase64->isChecked( ) ) {
		// usando Qt;
			pteMessages->appendPlainText(QString("< %1").arg(QString(base64_encode(msg.trimmed().toStdString()).c_str())));
			msg.clear();
		/*
		// usando funções adicionais;
			uint8_t i[2048];
			char o[2048];
			uint16_t s = uint16_t(message->size());
			memcpy(i, message->toStdString().c_str(),size_t(s));
			base64_decode(i, &s, o);
			QByteArray b = QByteArray(o, s);
			QString msg = QString(b.toHex());
			pteMessages->appendPlainText(QString("%1: %2").arg(++counter, 4).arg(msg.trimmed( ) ));

			memcpy(i, o, s);
			base64_encode(i, &s, o);
			b = QByteArray(o);
			msg = QString(b);
			pteMessages->appendPlainText(QString("original: %1").arg(msg.trimmed( ) ));

		*/
	} else {
		if (rbAscii->isChecked( ) ) {
			pteMessages->appendPlainText(QString("< %1").arg(QString(msg).trimmed()));
			msg.clear();
		} else {
			if (rbHex->isChecked()) {
				if (msg.size() > 6) {
					union {
						uint16_t integer;
						uint8_t bytes[2];
					} crc_;
					crc_.integer = crc_cal_by_bit((uint8_t *)(msg.data() + 2), msg.size()-4);
					uint8_t t1 = msg.at(msg.size() - 2);
					uint8_t t2 = msg.at(msg.size() - 1);
					if ( ( crc_.bytes[0] == t1 ) && ( crc_.bytes[1] == t2 ) ) {
						pteMessages->appendPlainText(QString("< %1").arg(QString(msg.toHex(','))));
						msg.clear();
					}
				}
			} else {
				pteMessages->appendPlainText(QString("< %1").arg("Unknown content."));
				msg.clear();
			}
		}
	}
}

inline bool QRcode::is_base64(char c) {
	return (isalnum(c) || (c == '+') || (c == '/' ) );
}

std::string QRcode::base64_encode(std::string const& string_to_encode) {
	std::string ret;
	int i = 0;
	int j = 0;
	char bytes_to_encode_buffer[1024];
	char * bytes_to_encode = bytes_to_encode_buffer;
	unsigned long in_len = string_to_encode.length();
	memcpy(bytes_to_encode,string_to_encode.c_str(),in_len);
	char char_array_3[3];
	uint8_t char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = static_cast<uint8_t>( ( (char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
			char_array_4[2] = static_cast<uint8_t>( ( (char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = static_cast<uint8_t>( ( (char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
		char_array_4[2] = static_cast<uint8_t>( ( (char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string QRcode::base64_decode(std::string const& string_to_decode) {
	uint64_t in_len = string_to_decode.size();
	int i = 0;
	int j = 0;
	uint64_t in_ = 0;
	uint8_t char_array_4[4], char_array_3[3];
	std::string ret;

	while ( ( (in_len--) != 0) && ( string_to_decode[in_] != char('=' ) ) && is_base64(char(string_to_decode[in_] ) )) {
		char_array_4[i++] = uint8_t(string_to_decode[in_]);
		in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = uint8_t(base64_chars.find(char(char_array_4[i] ) ));

			char_array_3[0] = uint8_t((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
			char_array_3[1] = uint8_t(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
			char_array_3[2] = uint8_t(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

			for (i = 0; (i < 3); i++)
				ret += char(char_array_3[i]);
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = uint8_t(base64_chars.find(char(char_array_4[j] ) ));

		char_array_3[0] = uint8_t((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
		char_array_3[1] = uint8_t(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
		char_array_3[2] = uint8_t(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

		for (j = 0; (j < i - 1); j++)
			ret += char(char_array_3[j]);
	}
	return ret;
}

void QRcode::base64_encode(uint8_t * in, uint16_t * size, char * out) {
	int i = 0;
	int j = 0;
	uint16_t idx1 = 0;
	uint16_t idx2 = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while ((*size)--) {
		char_array_3[i++] = in[idx1++];
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = uint8_t(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
			char_array_4[2] = uint8_t(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				out[idx2++] = base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = uint8_t(((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4));
		char_array_4[2] = uint8_t(((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6));

		for (j = 0; (j < i + 1); j++)
			out[idx2++] = base64_chars[char_array_4[j]];

		while((i++ < 3))
			out[idx2++] = '=';
	}
	out[idx2+1] = 0;
	*size = idx2;
}

void QRcode::base64_decode(uint8_t * in, uint16_t * size, char * out) {
	int i = 0;
	int j = 0;
	uint16_t idx1 = 0;
	uint16_t idx2 = 0;
	uint8_t char_array_4[4], char_array_3[3];
	memset(out,0,2048);
	memset(out,0,*size);	// the decoded buffer is allways smaller than the input buffer;

	while ( (*size)-- && ( in[idx1] != '=' ) && is_base64( char( in[idx1] ) ) ) {
		char_array_4[i++] = in[idx1++];
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = uint8_t(base64_chars.find(char(char_array_4[i] ) ));

			char_array_3[0] = uint8_t((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
			char_array_3[1] = uint8_t(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
			char_array_3[2] = uint8_t(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

			for (i = 0; (i < 3); i++) {
				out[idx2++] = char(char_array_3[i]);
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = uint8_t(base64_chars.find(char(char_array_4[j])));

		char_array_3[0] = uint8_t((char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4));
		char_array_3[1] = uint8_t(((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2));
		char_array_3[2] = uint8_t(((char_array_4[2] & 0x3) << 6) + char_array_4[3]);

		for (j = 0; (j < i - 1); j++)
			out[idx2++] = char(char_array_3[j]);
	}
	out[idx2+1] = 0;
	*size = idx2;
}


 uint16_t QRcode::crc_cal_by_bit(uint8_t * ptr, uint8_t len) {
	union crc {
		uint32_t integer = 0;
		struct __attribute__((packed)) shorts{
			union p1{
				uint16_t integer;
				uint8_t bytes[2];
			} p1;
			uint16_t p2;
		} shorts;
	} crc;

	while(len-- != 0)
	{
		for(unsigned char i = 0x80; i != 0; i /= 2) {
			crc.integer *= 2;
			if((crc.integer & 0x10000) !=0) //Last CRC * 2 ,if the first one is 1,so divide 0x1BUFFER_SIZE21
				crc.integer ^= 0x11021;
			if((*ptr&i) != 0) //If the standard is 1,so CRC = last CRC + standard CRC_CCITT
				crc.integer ^= 0x1021;
		}
		ptr++;
	}
	uint8_t p = crc.shorts.p1.bytes[1];
	crc.shorts.p1.bytes[1] = crc.shorts.p1.bytes[0];
	crc.shorts.p1.bytes[0] = p;
	return (crc.shorts.p1.integer);
}
