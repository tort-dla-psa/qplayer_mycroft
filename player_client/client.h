#pragma once
#include <QObject>
#include <QThread>
#include "client_socket.h"

class client:public QObject{
    Q_OBJECT

    QThread* sock_thr;
    client_socket* sock;
protected:
    QTextStream* str;
public:
    client(QTextStream* str, QObject* parent = nullptr);
public slots:
    void start();
    void on_recieve(QByteArray data);
signals:
    void closed();
    void got_data(QByteArray data);
};
