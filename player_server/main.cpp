#include <QCoreApplication>
#include <QTimer>
#include "server.h"

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

    server* srv = new server();
    QObject::connect(srv, &server::closed, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, srv, &server::start);
    return a.exec();
}
