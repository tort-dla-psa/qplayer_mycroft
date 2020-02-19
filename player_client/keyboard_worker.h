#pragma once
#include <QObject>
#include <QSharedPointer>
#include "commands.h"

class keyboard_worker:public QObject{
	Q_OBJECT
public:
	keyboard_worker(QObject* parent = nullptr);
public slots:
	void start();
signals:
	void got_command(QSharedPointer<command> data);
};