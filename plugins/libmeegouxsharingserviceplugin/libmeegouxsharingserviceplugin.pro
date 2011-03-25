include(../../common.pri)
unix { 
    PREFIX = /usr
    LIBDIR = $$PREFIX/lib
    INCLUDEDIR = $$PREFIX/include/meego-ux-sharing/plugin
}
TEMPLATE = lib
TARGET = meegouxsharingserviceplugin
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen
DEPENDPATH += .
CONFIG += qdbus link_pkgconfig \
	debug
PKGCONFIG += 

!exists(../../interfaces/meego-ux-sharing-daemon_adaptor.h) {
    system(cd ../../interfaces && ./generate-adaptors.sh)
}

INCLUDEPATH += ../../interfaces ../../common

INSTALL_HEADERS = ../../common/meegouxsharingserviceplugininterface.h \
    meegouxsharingservice.h \
    ../../interfaces/meego-ux-sharing-service_adaptor.h
HEADERS += $$INSTALL_HEADERS \
    ../../common/meegouxsharingdbustypes.h \
    ../../common/meegouxsharingcommon.h
SOURCES += ../../interfaces/meego-ux-sharing-service_adaptor.cpp \
    ../../common/meegouxsharingdbustypes.cpp \
    meegouxsharingservice.cpp
target.path = $$LIBDIR
headers.files = $$INSTALL_HEADERS
headers.path = $$INCLUDEDIR
pcfile.files = libmeegouxsharingserviceplugin.pc
pcfile.path = $$LIBDIR/pkgconfig
INSTALLS += target \
    headers \
    pcfile
OTHER_FILES += README libmeegouxsharingserviceplugin.pc

PKG_NAME = libmeegouxsharingserviceplugin
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

