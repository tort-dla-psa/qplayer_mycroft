#pragma once
#include <QObject>
#include <QThread>
#include "server_socket.h"

class server:public QObject{
    Q_OBJECT

    bool use_net;
    client* cli;
    QThread* cli_thr;
public:
    explicit server(bool use_net=true,
        QObject *parent = nullptr);
    ~server();
public slots:
    void start();
    void on_read(QByteArray data);
    void send(QByteArray data);
signals:
    void closed();
};
