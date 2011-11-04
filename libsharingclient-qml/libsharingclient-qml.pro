include(../common.pri)
TEMPLATE = lib
TARGET = Sharing
QT += declarative \
    dbus
CONFIG += qt \
    plugin \
    dbus \
    link_pkgconfig
PKGCONFIG += libsharingclient-qt sharing

TARGET = $$qtLibraryTarget($$TARGET)
DESTDIR = $$TARGET

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += ../libsharingclient-qt ../common

LIBS += -L../libsharingclient-qt -lsharingclient-qt

# Input
SOURCES += sharingclientqml.cpp \
    sharingclientqmlobj.cpp

HEADERS += sharingclientqml.h \
    sharingclientqmlobj.h

qmldir.files += $$TARGET
qmldir.path += $$[QT_INSTALL_IMPORTS]/
INSTALLS += qmldir

PKG_NAME = libsharingclient-qml
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

