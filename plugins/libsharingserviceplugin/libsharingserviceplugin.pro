include(../../common.pri)
unix { 
    PREFIX = /usr
    LIBDIR = $$PREFIX/lib
    INCLUDEDIR = $$PREFIX/include/sharing/plugin
}
TEMPLATE = lib
TARGET = sharingserviceplugin
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen
DEPENDPATH += .
CONFIG += qdbus link_pkgconfig \
	debug
PKGCONFIG += 

!exists(../../interfaces/sharing-daemon_adaptor.h) {
    system(cd ../../interfaces && ./generate-adaptors.sh)
}

INCLUDEPATH += ../../interfaces ../../common

INSTALL_HEADERS = ../../common/sharingserviceplugininterface.h \
    sharingservice.h \
    ../../interfaces/sharing-service_adaptor.h
HEADERS += $$INSTALL_HEADERS \
    ../../common/sharingdbustypes.h \
    ../../common/sharingcommon.h
SOURCES += ../../interfaces/sharing-service_adaptor.cpp \
    ../../common/sharingdbustypes.cpp \
    sharingservice.cpp
target.path = $$LIBDIR
headers.files = $$INSTALL_HEADERS
headers.path = $$INCLUDEDIR
pcfile.files = libsharingserviceplugin.pc
pcfile.path = $$LIBDIR/pkgconfig
INSTALLS += target \
    headers \
    pcfile
OTHER_FILES += README libsharingserviceplugin.pc

PKG_NAME = libsharingserviceplugin
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

