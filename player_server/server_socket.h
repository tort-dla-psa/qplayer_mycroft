#pragma once
#include <QTcpSocket>
#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QSharedPointer>
#include "multithread_printer.h"
#include "commands.h"

class client:public QObject{
	Q_OBJECT
public:
	explicit client(QObject *parent = nullptr);
	virtual ~client();
public slots:
	virtual void send_to_cli(QByteArray data)=0;
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

	inline void print(const QString &mes){
		multithread_printer::message m;
		m.thread_ptr = (unsigned long)QThread::currentThread();
		m.thread_id = QThread::currentThreadId();
		m.name = "net_cli";
		m.mes = mes;
		emit send_print(m);
	}
public:
	explicit net_client(QTcpSocket* sock, QObject *parent = nullptr);
	~net_client();
public slots:
	void send_to_cli(QByteArray data)override;
	void close()override;
	void start()override;
	void on_client_state_changed(QAbstractSocket::SocketState)override;
private slots:
	void on_ready_read()override;
	void on_disconnected()override;
signals:
	void send_print(multithread_printer::message);
};
