QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = player_client
INCLUDEPATH = ../common
SOURCES +=  ../common/commands.cpp \
		client_socket.cpp \
		client.cpp \
		main.cpp
HEADERS += ../common/commands.h \
	client_socket.h \
	client.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
