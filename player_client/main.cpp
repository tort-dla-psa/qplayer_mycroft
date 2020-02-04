#include <QCoreApplication>
#include <QTimer>
#include "client.h"

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

	client* cli = new client(&a);
    QObject::connect(cli, &client::closed, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, cli, &client::start);
    return a.exec();
}
