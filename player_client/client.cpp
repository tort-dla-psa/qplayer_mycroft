#include <QTextStream>
#include "client.h"
#include "commands.h"
#include "info.h"

client::client(QObject* parent)
	:QObject(parent)
{}

void client::start(){
	sock = new net_socket();
	QThread* sock_thr = new QThread();

	sock->moveToThread(sock_thr);
	connect(sock, &client_socket::recieved,
			this, &client::on_recieve);
	connect(sock_thr, &QThread::started,
			sock, &client_socket::start);
	connect(this, &client::got_data,
			sock, &client_socket::send);

	sock_thr->start();

	QTextStream qtin(stdin);
	while(true){
		QString word;
		qtin >> word;
		if(word == "list"){
			class list lst;
			emit got_data(lst.serialize());
		}
	}
}

void client::on_recieve(QByteArray data){
	qDebug()<<"got data:"<<QString::fromUtf8(data);
	if(data.front() == (char)magic_bytes_info::dir){
		class dir d;
		d.deserialize(data);
		qDebug()<<"dirs:";
		for(auto &d_:d.get_dirs()){
			qDebug()<<d_;
		}
		qDebug()<<"files:";
		for(auto &f_:d.get_files()){
			qDebug()<<f_;
		}
		return;
	}
}
