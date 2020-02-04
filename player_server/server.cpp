#include "server.h"
#include "commands.h"
#include "info.h"

server::server(QString path, bool use_net, QObject *parent)
	:QObject(parent)
{
	this->path = path;
	this->use_net = use_net;
	connect(this, &server::send_print,
			&printer, &multithread_printer::print);
}

server::~server(){
	//sock_thr->deleteLater();
}

void server::start(){
	print("making net_socket");
	if(use_net){
		QTcpServer* serv = new QTcpServer();
		serv->listen(QHostAddress::Any, 4242);
		serv->waitForNewConnection(-1);
		auto m_cli = serv->nextPendingConnection();
		print("new cli:"+m_cli->peerAddress().toString());
		cli = new net_client(m_cli);
		//serv->deleteLater();
	}

	connect(cli, &client::recieved,
			this, &server::on_read);
	connect((net_client*)cli, &net_client::send_print,
			&printer, &multithread_printer::print);
	cli->start();

	//player//
	wrkr = new play_worker();
	play_thread = new QThread();
	wrkr->moveToThread(play_thread);
	connect(wrkr, &play_worker::progress,
			this, &server::on_play_progress);
	connect(wrkr, &play_worker::send_print,
			&printer, &multithread_printer::print);
	connect(play_thread, &QThread::started,
			wrkr, &play_worker::start);
	connect(this, &server::send_cmd,
			wrkr, &play_worker::process_cmd);
	wrkr->open_dir(this->path);
	play_thread->start();
}

void server::on_read(QByteArray data){
	print("got data:"+QString::fromUtf8(data));
	QSharedPointer<command> cmd;
	if(data.front() == (char)magic_bytes_cmd::pause_byte){
		cmd = QSharedPointer<pause>::create();
	}else if(data.front() == (char)magic_bytes_cmd::rewind_byte){
		cmd = QSharedPointer<class rewind>::create();
	}else{
		print("unknown command");
		return;
	}
	cmd->deserialize(data);
	emit send_cmd(cmd);
}
void server::send(QByteArray data){
	print("sending data:"+QString::fromUtf8(data));
	emit send_to_cli(data);
}
void server::on_play_progress(int progress){
	print("play progress:"+QString::number(progress));
	class progress prgs;
	prgs.set_progress(progress);
	send(prgs.serialize());
}
