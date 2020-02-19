#include <QTextStream>
#include <QDataStream>
#include "client.h"
#include "commands.h"
#include "info.h"

client::client(QObject* parent)
	:QObject(parent)
{}

void client::start(){
	sock = new net_socket();
	//this->sock_thr = new QThread();

	//sock->moveToThread(sock_thr);
	connect(sock, &client_socket::recieved,
			this, &client::on_recieve);
	//connect(sock_thr, &QThread::started,
	//		sock, &client_socket::start);
	sock->start();

	kbd = new keyboard_worker();
	this->kbd_thr = new QThread();
	kbd->moveToThread(kbd_thr);
	connect(kbd_thr, &QThread::started,
		kbd, &keyboard_worker::start);
	connect(kbd, &keyboard_worker::got_command,
			this, &client::process_kbd_command);
	//sock_thr->start();
	kbd_thr->start();
}

void client::on_recieve(QByteArray data){
	QDataStream str(&data, QIODevice::ReadOnly);
	qint8 ch;
	str >> ch;

	if(ch == (qint8)magic_bytes_info::dir){
		class dir d;
		str >> d;
		qDebug()<<"dirs:";
		for(auto &d_:d.get_dirs()){
			qDebug()<<d_;
			files.push_back(d_);
		}
		qDebug()<<"files:";
		for(auto &f_:d.get_files()){
			qDebug()<<f_;
			files.push_back(f_);
		}
		return;
	}
}

void client::process_kbd_command(QSharedPointer<command> cmd){
	QByteArray data;
	QDataStream str(&data, QIODevice::WriteOnly);
	{
		auto opn = cmd.dynamicCast<class open>();
		if(opn){
			if(opn->get_path() == ".."){
			}else{
				QString index_str = opn->get_path();
				int index = index_str.toInt();
				opn->set_path(this->files.at(2+index));
			}
			str << *opn;
			sock->send(data);
			return;
		}
	}{
		auto lst = cmd.dynamicCast<class list>();
		if(lst){
			str << *lst;
			sock->send(data);
			return;
		}
	}
}