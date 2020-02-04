#include <QString>
#include "multithread_printer.h"

multithread_printer::multithread_printer(QObject *parent) : QObject(parent) {
}

void multithread_printer::print(multithread_printer::message m){
	QString str;
	str.setNum(m.thread_ptr, 16);
	qDebug()<<"thr:"<<m.thread_id
			<<" thr_ptr:"<<str
			<<" ["<<m.name<<"]:"
			<<m.mes;
}
