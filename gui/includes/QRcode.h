#ifndef QRCODE_H
#define QRCODE_H

#include "ui_QRcode.h"
#include "QRcodeComm.h"
#include <QString>

class QRcode : public QWidget, private Ui::QRcode
{
	Q_OBJECT

public:
	explicit QRcode(QWidget *parent = nullptr);
	~QRcode();

protected:
	void changeEvent(QEvent *e);

private:
	QRcodeComm	* serialPort;
	QString		mode;
	bool		alreadyOpened;
	qint16		counter;

protected
slots:
	void on_pbOpenClose_clicked();
	void on_pbClear_clicked();
	void on_leDevice_editingFinished();
	void on_cbBaud_currentIndexChanged(QString text);
	void on_cbBits_currentIndexChanged(QString text);
	void on_cbParity_currentIndexChanged(QString text);
	void on_cbStopBits_currentIndexChanged(QString text);
	void on_cbFlow_currentIndexChanged(QString text);
	void on_leTimeOut_editingFinished();
	void slotMessageReceived(QString * message);
};

#endif // QRCODE_H
