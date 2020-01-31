#include "server.h"

server::server(bool use_net, QObject *parent)
    :QObject(parent)
{
    this->use_net = use_net;
}

server::~server(){
    //sock_thr->deleteLater();
}

void server::start(){
    qDebug()<<"making net_socket\n";
    if(use_net){
        QTcpServer* serv = new QTcpServer();
        serv->listen(QHostAddress::Any, 4242);
        serv->waitForNewConnection(-1);
        auto m_cli = serv->nextPendingConnection();
        qDebug()<<"new cli:"<<m_cli->peerAddress().toString()<<"\n";
        cli = new net_client(m_cli);
        //serv->deleteLater();
    }
    QThread* sock_thr = new QThread();
    cli->moveToThread(sock_thr);

    connect(sock_thr, &QThread::started,
            cli, &client::start);
    connect(cli, &client::recieved,
            this, &server::on_read);
    sock_thr->start();

    while(true){
        //simulate playing
        qDebug()<<"working\n";
        QThread::currentThread()->sleep(2);
    }
}

void server::on_read(QByteArray data){
    qDebug()<<"got data:"<<QString::fromUtf8(data)<<"\n";
    send(data);
}
void server::send(QByteArray data){
    qDebug()<<"sending data:"<<QString::fromUtf8(data)<<"\n";
    cli->send(data);
}
