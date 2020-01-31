#pragma once
#include <QTcpSocket>
#include <QObject>
#include <QTcpServer>

class client:public QObject{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr);
    virtual ~client();
public slots:
    virtual void send(QByteArray data)=0;
    virtual void close()=0;
    virtual void start()=0;
    virtual void on_client_state_changed(QAbstractSocket::SocketState)=0;
private slots:
    virtual void on_ready_read()=0;
    virtual void on_disconnected()=0;
signals:
    void disconnected();
    void recieved(QByteArray data);
};

class net_client:public client{
    Q_OBJECT

    QTcpSocket* sock;
public:
    explicit net_client(QTcpSocket* sock, QObject *parent = nullptr);
    ~net_client();
public slots:
    void send(QByteArray data)override;
    void close()override;
    void start()override;
    void on_client_state_changed(QAbstractSocket::SocketState)override;
private slots:
    void on_ready_read()override;
    void on_disconnected()override;
signals:
};
