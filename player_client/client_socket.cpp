#include "client_socket.h"

client_socket::client_socket(QObject *parent)
	:QObject(parent)
{ }
client_socket::~client_socket(){}

//net_client//
net_socket::net_socket(QObject *parent)
	:client_socket(parent)
{}
net_socket::~net_socket(){}

void net_socket::send(QByteArray data){
	qint64 counter = 0;
	while(counter != data.size()){
		auto count = sock->write(data.data()+counter, data.size()-counter);
		if(count <= 0){
			qDebug()<<sock->error()<<"\n";
			throw std::runtime_error("error sending bytes");
		}
		counter += count;
	}
}

void net_socket::start(){
	qDebug("connecting to port %i",port);
	sock = new QTcpSocket();
	sock->connectToHost("127.0.0.1", port);
	sock->waitForConnected(-1);
	qDebug("connected to port %i",port);
	connect(sock, &QTcpSocket::readyRead,
			this, &net_socket::on_ready_read);
}

void net_socket::on_ready_read(){
	auto bytes = sock->readAll();
	emit recieved(bytes);
}
