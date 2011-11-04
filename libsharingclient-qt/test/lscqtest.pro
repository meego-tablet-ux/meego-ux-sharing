TEMPLATE = app
QT += dbus
CONFIG += link_pkgconfig
PKGCONFIG += sharing \
    libsharingclient-qt

TARGET = lscqtest

INCLUDEPATH += /usr/include/sharing

target.path=/usr/bin

LIBRARY+=-lsharingclient-qt


HEADERS += lscqtest.h

SOURCES += main.cpp \
    lscqtest.cpp

