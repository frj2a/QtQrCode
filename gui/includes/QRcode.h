#ifndef QRCODE_H
#define QRCODE_H

#include "ui_QRcode.h"
#include "QRcodeComm.h"
#include <QString>
#include <QMap>

#include<string.h>
#include <string>

class QRcode : public QWidget, private Ui::QRcode {
	Q_OBJECT

public:
	explicit QRcode(QWidget *parent = nullptr);
	~QRcode();

protected:
	void changeEvent(QEvent *e);

private:
	QRcodeComm	* serialPort;
	QString						mode;
	bool						alreadyOpened;
	bool						waitTerminator;
	unsigned char				terminator;
	QMap<QString, QByteArray>	messages;
	QByteArray					msg;

	inline bool is_base64(char c);
	std::string base64_encode(std::string const& string_to_encode);
	std::string base64_decode(std::string const& string_to_decode);
	void base64_encode(uint8_t * in, uint16_t * size, char * out);
	void base64_decode(uint8_t * in, uint16_t * size, char * out);
	static uint16_t crc_cal_by_bit(uint8_t * ptr, uint8_t len);

protected
slots:
	void on_pbOpenClose_clicked();
	void on_pbClear_clicked();
	void on_pbSend_clicked();
	void on_leDevice_editingFinished();
	void on_cbBaud_currentIndexChanged(QString text);
	void on_cbBits_currentIndexChanged(QString text);
	void on_cbParity_currentIndexChanged(QString text);
	void on_cbStopBits_currentIndexChanged(QString text);
	void on_cbFlow_currentIndexChanged(QString text);
	void on_leTimeOut_editingFinished();
	void on_rbAscii_clicked();
	void on_rbBase64_clicked();
	void on_rbHex_clicked();
	void slotMessageReceived(QByteArray *message);
};

#endif // QRCODE_H
