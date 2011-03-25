TEMPLATE = app
QT += dbus
CONFIG += link_pkgconfig
PKGCONFIG += meego-ux-sharing \
    libmeegouxsharingclient

TARGET = lmsctest

INCLUDEPATH += /usr/include/meego-ux-sharing

target.path=/usr/bin

LIBRARY+=-lmeegouxsharingclient


HEADERS += lmsctest.h

SOURCES += main.cpp \
    lmsctest.cpp

