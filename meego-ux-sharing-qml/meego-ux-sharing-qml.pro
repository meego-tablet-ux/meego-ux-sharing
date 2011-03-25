include(../common.pri)
TEMPLATE = lib
TARGET = Sharing
QT += declarative \
    dbus
CONFIG += qt \
    plugin \
    dbus \
    link_pkgconfig
PKGCONFIG += libmeegouxsharingclient meego-ux-sharing

TARGET = $$qtLibraryTarget($$TARGET)
DESTDIR = $$TARGET

OBJECTS_DIR = .obj
MOC_DIR = .moc

INCLUDEPATH += ../libmeegouxsharingclient ../common

LIBS += -L../libmeegouxsharingclient -lmeegouxsharingclient

# Input
SOURCES += meegouxsharingclientqml.cpp \
    meegouxsharingclientqmlobj.cpp

HEADERS += meegouxsharingclientqml.h \
    meegouxsharingclientqmlobj.h

qmldir.files += $$TARGET
qmldir.path += $$[QT_INSTALL_IMPORTS]/MeeGo/
INSTALLS += qmldir

PKG_NAME = meego-ux-sharing-qml
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

