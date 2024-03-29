include(../common.pri)
TEMPLATE = app
QT += dbus

TARGET = sharingd

INCLUDEPATH += ../common ../interfaces

!exists(../interfaces/sharing-daemon_adaptor.h) {
    system(cd ../interfaces && ./generate-adaptors.sh)
}

target.path=/usr/libexec

HEADERS += ../interfaces/*_adaptor.h \ 
    sharingdaemon.h

SOURCES += ../interfaces/*_adaptor.cpp \
    main.cpp \
    sharingdaemon.cpp


# DBUS Service files
dbus_service.files = share/dbus-1/services/*
dbus_service.path = /usr/share/dbus-1/services

INSTALLS += target dbus_service

PKG_NAME = sharing
TRANSLATIONS += $${HEADERS} \
	$${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist
