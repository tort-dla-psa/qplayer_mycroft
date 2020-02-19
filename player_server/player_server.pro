QT -= gui
QT += network multimedia

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = player_server
INCLUDEPATH = ../common
SOURCES += server.cpp \
		server_socket.cpp \
		play_worker.cpp \
		main.cpp \
		../common/commands.cpp \
		../common/multithread_printer.cpp

HEADERS += server.h \
		server_socket.h \
		play_worker.h \
		../common/commands.h \
		../common/info.h \
		../common/serializable.h \
		../common/multithread_printer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
