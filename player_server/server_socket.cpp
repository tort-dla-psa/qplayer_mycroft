#include "server_socket.h"

client::client(QObject *parent)
	:QObject(parent)
{}
client::~client(){}

net_client::net_client(QTcpSocket* sock, QObject *parent)
	:client(parent)
{
	this->sock = sock;
	connect(sock, &QTcpSocket::stateChanged,
			this, &net_client::on_client_state_changed);
	//connect(sock, &QTcpSocket::readyRead,
	//		this, &net_client::on_ready_read);
	connect(sock, &QTcpSocket::disconnected,
			this, &net_client::on_disconnected);
}
net_client::~net_client(){
	print("destroyed");
}

void net_client::send_to_cli(QByteArray data){
	qint64 counter = 0;
	while(counter != data.size()){
		auto count = sock->write(data.data()+counter, data.size()-counter);
		counter+=count;
	}
}

void net_client::close(){ }
void net_client::start(){
	connect(sock, &QTcpSocket::readyRead,
		this, &net_client::on_ready_read);
}
void net_client::on_client_state_changed(QAbstractSocket::SocketState state){}
void net_client::on_ready_read(){
	auto bytes = sock->readAll();
	emit recieved(bytes);
}
void net_client::on_disconnected(){
	sock->deleteLater();
	emit disconnected();
}
