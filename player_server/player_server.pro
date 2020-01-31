QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = player_server
INCLUDEPATH = ../common
SOURCES +=  ../common/commands.cpp \
		server.cpp \
		server_socket.cpp \
		main.cpp
HEADERS += server.h \
		server_socket.h \
		../common/commands.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
