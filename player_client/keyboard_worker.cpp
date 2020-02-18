#include "keyboard_worker.h"
#include <QTextStream>
#include <QString>
#include "info.h"
#include "commands.h"

keyboard_worker::keyboard_worker(QObject* parent)
    :QObject(parent)
{}
void keyboard_worker::start(){
	QTextStream qtin(stdin);
	while(true){
		QString word;
		qtin >> word;
		if(word == "list"){
			class list lst;
			emit got_data(lst.serialize());
		}
	}
}