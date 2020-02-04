#pragma once
#include <QObject>
#include <QThread>
#include <QString>
#include "server_socket.h"
#include "play_worker.h"
#include "utils.h"
#include "multithread_printer.h"
#include "commands.h"

class server:public QObject{
	Q_OBJECT

	bool use_net;
	client* cli;
	play_worker* wrkr;
	QThread* play_thread;

	multithread_printer printer;

	inline void print(const QString &mes){
		multithread_printer::message m;
		m.thread_ptr = (unsigned long)QThread::currentThread();
		m.thread_id = QThread::currentThreadId();
		m.name = "server";
		m.mes = mes;
		emit send_print(m);
	}
	QString path;
public:
	explicit server(QString path, bool use_net=true,
					QObject *parent = nullptr);
	~server();
public slots:
	void start();
	void on_read(QByteArray data);
	void send(QByteArray data);
	void on_play_progress(int progress);
signals:
	void closed();
	void send_to_cli(QByteArray cmd);
	void send_cmd(QSharedPointer<command> cmd);
	void send_print(multithread_printer::message);
};
