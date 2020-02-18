#include <QTextStream>
#include "client.h"
#include "commands.h"
#include "info.h"

client::client(QObject* parent)
	:QObject(parent)
{}

void client::start(){
	sock = new net_socket();
	this->sock_thr = new QThread();

	sock->moveToThread(sock_thr);
	connect(sock, &client_socket::recieved,
			this, &client::on_recieve);
	connect(sock_thr, &QThread::started,
			sock, &client_socket::start);

	kbd = new keyboard_worker();
	this->kbd_thr = new QThread();
	kbd->moveToThread(kbd_thr);
	connect(kbd_thr, &QThread::started,
		kbd, &keyboard_worker::start);
	connect(kbd, &keyboard_worker::got_data,
			sock, &client_socket::send);
	sock_thr->start();
	kbd_thr->start();
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
