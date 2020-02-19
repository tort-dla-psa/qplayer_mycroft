#include "keyboard_worker.h"
#include <QSharedPointer>
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
		QSharedPointer<command> cmd;
		if(word == "list"){
			cmd = QSharedPointer<class list>::create();
		}else if(word.count("open")!=0){
			auto opn = QSharedPointer<class open>::create();
			if(word == "open .."){
				opn->set_path("..");
				cmd = opn;
			}else{
				QRegExp rx("open (\\d+)"); //open 2
				if(rx.exactMatch(word)){
					opn->set_path(rx.cap());
					cmd = opn;
				}
			}
		}
		if(cmd){
			emit got_command(cmd);
		}
	}
}
