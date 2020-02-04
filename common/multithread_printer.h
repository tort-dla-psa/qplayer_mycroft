#pragma once

#include <QObject>
#include <QDebug>
#include <QThread>

class multithread_printer:public QObject{
	Q_OBJECT
public:
	struct message{
		Qt::HANDLE thread_id;
		unsigned long thread_ptr;
		QString name;
		QString mes;
	};
	explicit multithread_printer(QObject *parent = nullptr);
public slots:
	void print(message m);
signals:
};
