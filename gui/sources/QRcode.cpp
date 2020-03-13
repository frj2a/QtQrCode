#include "QRcode.h"


#include <string>

std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
void base64_encode(uint8_t * in, uint16_t * size, char * out);
void base64_decode(uint8_t * in, uint16_t * size, char * out);


#include<string.h>

static const std::string base64_chars =	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
										"abcdefghijklmnopqrstuvwxyz"
										"0123456789+/";

static inline bool is_base64(char c) {
	return (isalnum(c) || (c == '+') || (c == '/' ) );
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	std::string ret;
	int i = 0;
	int j = 0;
	uint8_t char_array_3[3];
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

std::string base64_decode(std::string const& encoded_string) {
	uint64_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	uint64_t in_ = 0;
	uint8_t char_array_4[4], char_array_3[3];
	std::string ret;

	while ( ( (in_len--) != 0) && ( encoded_string[in_] != char('=' ) ) && is_base64(char(encoded_string[in_] ) )) {
		char_array_4[i++] = uint8_t(encoded_string[in_]);
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

void base64_encode(uint8_t * in, uint16_t * size, char * out) {
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

void base64_decode(uint8_t * in, uint16_t * size, char * out) {
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



QRcode::QRcode(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	serialPort = new QRcodeComm();
	connect(serialPort, &QRcodeComm::signalMessageReceived, this, &QRcode::slotMessageReceived);
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
	alreadyOpened = false;
	counter = 0;
}

QRcode::~QRcode()
{
	disconnect(serialPort, &QRcodeComm::signalMessageReceived, this, &QRcode::slotMessageReceived);
}

void QRcode::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type( ) )
	{
		case QEvent::LanguageChange:
			retranslateUi(this);
		break;
		default:
		break;
	}
}

void QRcode::on_pbOpenClose_clicked()
{
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
	if (serialPort->isSerialPortOpened( ) )
	{
		serialPort->closeSerialPort();
		leDevice->setEnabled(true);
		cbBaud->setEnabled(true);
		cbBits->setEnabled(true);
		cbParity->setEnabled(true);
		cbStopBits->setEnabled(true);
		cbFlow->setEnabled(true);
		leTimeOut->setEnabled(true);
		pbOpenClose->setText(tr("&Open" ) );
	}
	else
	{
		if (alreadyOpened)
		{
			serialPort->reopenSerialPort();
		}
		else
		{
			unsigned int timeOut = leTimeOut->text().toUInt();
			if (timeOut > 255)
			{
				timeOut = 255;
			}
			std::string dev = leDevice->text().toStdString();
			int32_t baud = cbBaud->currentText().toInt();
			std::string smode = mode.toStdString();
			cc_t tout = cc_t(timeOut);
			serialPort->openSerialPort(dev, baud, smode, tout);
		}
		if (serialPort->isSerialPortOpened( ) )
		{
			leDevice->setEnabled(false);
			cbBaud->setEnabled(false);
			cbBits->setEnabled(false);
			cbParity->setEnabled(false);
			cbStopBits->setEnabled(false);
			cbFlow->setEnabled(false);
			leTimeOut->setEnabled(false);
			pbOpenClose->setText(tr("&Close" ) );
		}
	}
}

void QRcode::on_pbClear_clicked()
{
	pteMessages->clear();
	counter = 0;
}

void QRcode::on_leDevice_editingFinished()
{
	alreadyOpened = false;
}

void QRcode::on_cbBaud_currentIndexChanged(QString)
{
	alreadyOpened = false;
}

void QRcode::on_cbBits_currentIndexChanged(QString text)
{
	alreadyOpened = false;
	mode = text + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::on_cbParity_currentIndexChanged(QString text)
{
	alreadyOpened = false;
	mode = cbBits->currentText() + text + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::on_cbStopBits_currentIndexChanged(QString text)
{
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + text + cbFlow->currentText();
}

void QRcode::on_cbFlow_currentIndexChanged(QString text)
{
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + text;
}

void QRcode::on_leTimeOut_editingFinished()
{
	alreadyOpened = false;
	mode = cbBits->currentText() + cbParity->currentText() + cbStopBits->currentText() + cbFlow->currentText();
}

void QRcode::slotMessageReceived(QString * message)
{
	if (rbBase64->isChecked( ) ) {
		{	// usando Qt;
			QByteArray b = QByteArray::fromBase64(QByteArray(message->toStdString().c_str( ) ));
			QString msg = QString(b.toHex());
			pteMessages->appendPlainText(QString("%1: %2").arg(++counter, 4).arg(msg.trimmed( ) ));
		}
		/*
		{	// usando funções adicionais;
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
		}
		*/
	} else {
		if (rbAscii->isChecked( ) ) {
			pteMessages->appendPlainText(QString("%1: %2").arg(++counter, 4).arg(message->trimmed( ) ));
		} else {
			pteMessages->appendPlainText(QString("%1: %2").arg(++counter, 4).arg("Unknown content." ) );
		}
	}
}
