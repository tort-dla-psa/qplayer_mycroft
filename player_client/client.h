#pragma once
#include <QObject>
#include <QThread>
#include <QDir>
#include <QSharedPointer>
#include "client_socket.h"
#include "multithread_printer.h"
#include "keyboard_worker.h"
#include "commands.h"

class client:public QObject{
	Q_OBJECT

	QThread* sock_thr,* kbd_thr;
	client_socket* sock;
	keyboard_worker* kbd;

	inline void print(const QString &mes){
		multithread_printer::message m;
		m.name = "client";
		m.thread_id = QThread::currentThreadId();
		auto id = (unsigned long)QThread::currentThread();
		m.thread_ptr = id;
		m.mes = mes;
		emit send_print(m);
	}

	QVector<QString> files;
public:
	client(QObject* parent = nullptr);
public slots:
	void start();
	void on_recieve(QByteArray data);
private slots:
	void process_kbd_command(QSharedPointer<command> cmd);
signals:
	void closed();
	void got_data(QByteArray data);
	void send_print(multithread_printer::message mes);
};
