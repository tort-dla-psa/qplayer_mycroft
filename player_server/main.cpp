#include <QCoreApplication>
#include <QTimer>
#include "server.h"
#include "multithread_printer.h"

Q_DECLARE_METATYPE(multithread_printer::message);
int main(int argc, char *argv[]){
	QCoreApplication a(argc, argv);
	qRegisterMetaType<multithread_printer::message>();
	qRegisterMetaType<QSharedPointer<command>>();

	server* srv = new server(a.arguments()[1]);
	QObject::connect(srv, &server::closed, &a, &QCoreApplication::quit);
	QTimer::singleShot(0, srv, &server::start);
	return a.exec();
}
