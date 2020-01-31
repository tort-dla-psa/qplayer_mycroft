#include "server_socket.h"

client::client(QObject *parent)
    :QObject(parent)
{}
client::~client(){}

net_client::net_client(QTcpSocket* sock, QObject *parent)
    :client(parent),
      sock(sock)
{
    connect(sock, &QTcpSocket::stateChanged,
            this, &net_client::on_client_state_changed);
    connect(sock, &QTcpSocket::readyRead,
            this, &net_client::on_ready_read);
    connect(sock, &QTcpSocket::disconnected,
               sock, &QObject::deleteLater);
}
net_client::~net_client(){
    qDebug()<<"destroyed\n";
}

void net_client::send(QByteArray data){
    qint64 counter = 0;
    qDebug()<<"sending bytes:"<<QString::fromUtf8(data)<<"\n";
    while(counter != data.size()){
        auto count = sock->write(data.data()+counter, data.size()-counter);
        counter+=count;
    }
}
void net_client::close(){ }
void net_client::start(){
    while(true){
        sock->waitForReadyRead(-1);
    }
}
void net_client::on_client_state_changed(QAbstractSocket::SocketState state){}
void net_client::on_ready_read(){
    qDebug()<<"got bytes:";
    auto bytes = sock->readAll();
    qDebug()<<QString::fromUtf8(bytes)<<"\n";
    emit recieved(bytes);
    send(bytes);
}
void net_client::on_disconnected(){
    emit disconnected();
}
