#include "QRcode.h"
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(QRcode);
	QRcode w;
	w.show();

	return a.exec();
}
