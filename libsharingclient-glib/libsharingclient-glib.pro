include(../common.pri)
unix { 
    PREFIX = /usr
    LIBDIR = $$PREFIX/lib
    INCLUDEDIR = $$PREFIX/include/meego-ux-sharing/client
}
TEMPLATE = lib
TARGET = sharingclient-glib
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen
DEPENDPATH += .
CONFIG += link_pkgconfig \
	  debug
PKGCONFIG += glib-2.0
QT -= core gui

INCLUDEPATH += ../interfaces ../common

INSTALL_HEADERS = \
    lscClient.h \
    lscCommon.h \
    lscService.h

HEADERS += \
    $$INSTALL_HEADERS \
    lscService_p.h

SOURCES += \
    lscService.c \
    lscClient.c

target.path = $$LIBDIR
headers.files = $$INSTALL_HEADERS
headers.path = $$INCLUDEDIR
pcfile.files = libsharingclient-glib.pc
pcfile.path = $$LIBDIR/pkgconfig
INSTALLS += target \
    headers \
    pcfile
OTHER_FILES += README

PKG_NAME = libsharingclient-glib
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist
