#include "server.h"
#include "commands.h"
#include "info.h"

server::server(QString path, bool use_net, QObject *parent)
	:QObject(parent)
{
	this->dir = QDir(path);
	if(!dir.exists()){
		throw std::runtime_error("no such dir");
	}
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
	connect(this, &server::send_to_cli,
			cli, &client::send_to_cli);
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
	wrkr->open_dir(this->dir.path());
	play_thread->start();
}

void server::on_read(QByteArray data){
	print("got data:"+QString::fromUtf8(data));
	QSharedPointer<command> cmd;
	if(data.front() == (char)magic_bytes_cmd::pause_byte){
		cmd = QSharedPointer<pause>::create();
	}else if(data.front() == (char)magic_bytes_cmd::rewind_byte){
		cmd = QSharedPointer<class rewind>::create();
	}else if(data.front() == (char)magic_bytes_cmd::list_dir_byte){
		cmd = QSharedPointer<class list>::create();
		list_dir();
		return;
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
void server::on_artist_change(artist a){
	print("artist changed:"+a.get_name());
	send(a.serialize());
}

void server::list_dir(){
	QVector<QString> dirs, files;
	QDir directory;
	directory.setPath(this->dir.path());
	directory.setFilter(QDir::Files | QDir::Dirs);
	directory.setSorting(QDir::Type);
	for(auto entry:directory.entryInfoList()){
		if(entry.isDir()){
			dirs.push_back(entry.fileName());
		}else if(entry.isFile()){
			files.push_back(entry.fileName());
		}
	}
	class dir info;
	info.set_dirs(dirs);
	info.set_files(files);
	send(info.serialize());
}