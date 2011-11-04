include(../common.pri)
unix { 
    PREFIX = /usr
    LIBDIR = $$PREFIX/lib
    INCLUDEDIR = $$PREFIX/include/sharing/client-qt
}
QT += declarative
TEMPLATE = lib
TARGET = sharingclient-qt
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen
DEPENDPATH += .
CONFIG += qdbus link_pkgconfig \
	debug
PKGCONFIG += sharing

INCLUDEPATH += ../interfaces ../common

# Having to do this by hand right now, as qdbusxml2cpp has some bugs in it...
!exists(../interfaces/sharing-service_interface.h) {
    system(cd ../interfaces && ./generate-interfaces.sh)
}

INSTALL_HEADERS = sharingclient.h \
    sharingclientservicemodel.h \
    sharingclientservice.h


HEADERS += ../interfaces/*_interface.h \
    sharingclientcore.h \
    $$INSTALL_HEADERS \
    ../common/sharingdbustypes.h

SOURCES += ../interfaces/*_interface.cpp \
    sharingclient.cpp \
    ../common/sharingdbustypes.cpp \
    sharingclientservicemodel.cpp \
    sharingclientcore.cpp \
    sharingclientservice.cpp

target.path = $$LIBDIR
headers.files = $$INSTALL_HEADERS
headers.path = $$INCLUDEDIR
pcfile.files = libsharingclient-qt.pc
pcfile.path = $$LIBDIR/pkgconfig
INSTALLS += target \
    headers \
    pcfile
OTHER_FILES += README

PKG_NAME = libsharingclient-qt
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist
