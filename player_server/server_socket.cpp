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
	print("sending bytes:"+QString::fromUtf8(data));
	while(counter != data.size()){
		auto count = sock->write(data.data()+counter, data.size()-counter);
		counter+=count;
	}
}

void net_client::close(){ }
void net_client::start(){
	while(true){
		print("waiting for bytes");
		sock->waitForReadyRead(-1);
		auto bytes = sock->readAll();
		print("got bytes:"+QString::fromUtf8(bytes));
		emit recieved(bytes);
		//send(bytes);
	}
}
void net_client::on_client_state_changed(QAbstractSocket::SocketState state){}
void net_client::on_ready_read(){
}
void net_client::on_disconnected(){
	sock->deleteLater();
	emit disconnected();
}
