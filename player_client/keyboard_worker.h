#pragma once
#include <QObject>

class keyboard_worker:public QObject{
	Q_OBJECT
public:
	keyboard_worker(QObject* parent = nullptr);
public slots:
	void start();
signals:
	void got_data(QByteArray data);
};