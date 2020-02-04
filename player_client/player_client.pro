QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = player_client
INCLUDEPATH = ../common
SOURCES +=   client_socket.cpp \
		client.cpp \
		main.cpp \
		../common/commands.cpp \
		../common/multithread_printer.cpp

HEADERS += client_socket.h \
	client.h \
	../common/commands.h \
	../common/multithread_printer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
