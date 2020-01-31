#pragma once
#include <QTcpSocket>
#include <QByteArray>
#include <QObject>
#include <QThread>

class client_socket : public QObject {
    Q_OBJECT
protected:
public:
    explicit client_socket(QObject *parent = nullptr);
    virtual ~client_socket();
public slots:
    virtual void send(QByteArray data)=0;
    virtual void start()=0;
signals:
    void recieved(QByteArray data);
private slots:
    virtual void on_ready_read()=0;
};

class net_socket:public client_socket{
    Q_OBJECT

    QTcpSocket* sock;
    int port = 4242;
public:
    explicit net_socket(QObject *parent = nullptr);
    ~net_socket();
public slots:
    void send(QByteArray data)override;
    void start()override;
private slots:
    void on_ready_read()override;
};
