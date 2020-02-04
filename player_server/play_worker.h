#pragma once
#include <QObject>
#include <QDir>
#include <QSharedPointer>
#include "multithread_printer.h"
#include "commands.h"

class play_worker:public QObject{
	Q_OBJECT
	QDir dir;
	bool dir_set;
	int m_progress;
	int update_usleep;
	bool paused;

	inline void print(const QString &mes){
		multithread_printer::message m;
		m.thread_ptr = (unsigned long)QThread::currentThread();
		m.thread_id = QThread::currentThreadId();
		m.name = "play_worker";
		m.mes = mes;
		emit send_print(m);
	}
public:
	explicit play_worker(QObject *parent = nullptr);
	int get_progress()const;
	bool get_paused()const;
public slots:
	void open_dir(QString path);
	void start();
	void rewind(int);
	void pause();
	void process_cmd(QSharedPointer<command> data);
	//void set_progress(int);
signals:
	void progress(int);
	void send_print(multithread_printer::message);
};
