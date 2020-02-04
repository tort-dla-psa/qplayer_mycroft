#include <QTextStream>
#include "client.h"

client::client(QObject* parent)
	:QObject(parent)
{}

void client::start(){
	sock = new net_socket();
	QThread* sock_thr = new QThread();

	connect(sock, &client_socket::recieved,
			this, &client::on_recieve);
	connect(sock_thr, &QThread::started,
			sock, &client_socket::start);
	connect(this, &client::got_data,
			sock, &client_socket::send);

	sock->moveToThread(sock_thr);
	sock_thr->start();

	QTextStream qtin(stdin);
	while(true){
		QString word;
		qtin >> word;
		emit got_data(word.toUtf8());
	}
}

void client::on_recieve(QByteArray data){
	qDebug()<<"got data:"<<QString::fromUtf8(data);
}
