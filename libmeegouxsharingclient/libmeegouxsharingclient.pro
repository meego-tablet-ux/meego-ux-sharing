include(../common.pri)
unix { 
    PREFIX = /usr
    LIBDIR = $$PREFIX/lib
    INCLUDEDIR = $$PREFIX/include/meego-ux-sharing/client
}
QT += declarative
TEMPLATE = lib
TARGET = meegouxsharingclient
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen
DEPENDPATH += .
CONFIG += qdbus link_pkgconfig \
	debug
PKGCONFIG += meego-ux-sharing

INCLUDEPATH += ../interfaces ../common

# Having to do this by hand right now, as qdbusxml2cpp has some bugs in it...
!exists(../interfaces/meego-ux-sharing-service_interface.h) {
    system(cd ../interfaces && ./generate-interfaces.sh)
}

INSTALL_HEADERS = meegouxsharingclient.h \
    meegouxsharingclientservicemodel.h \
    meegouxsharingclientservice.h


HEADERS += ../interfaces/*_interface.h \
    meegouxsharingclientcore.h \
    $$INSTALL_HEADERS \
    ../common/meegouxsharingdbustypes.h

SOURCES += ../interfaces/*_interface.cpp \
    meegouxsharingclient.cpp \
    ../common/meegouxsharingdbustypes.cpp \
    meegouxsharingclientservicemodel.cpp \
    meegouxsharingclientcore.cpp \
    meegouxsharingclientservice.cpp

target.path = $$LIBDIR
headers.files = $$INSTALL_HEADERS
headers.path = $$INCLUDEDIR
pcfile.files = libmeegouxsharingclient.pc
pcfile.path = $$LIBDIR/pkgconfig
INSTALLS += target \
    headers \
    pcfile
OTHER_FILES += README

PKG_NAME = libmeegouxsharingclient
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist
