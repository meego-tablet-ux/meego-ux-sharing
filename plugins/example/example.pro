exists(../../common.pri) {
	include(../../common.pri)
} else {
  MSD_PLUGIN_PATH=/usr/lib/sharing/plugins/
  PLUGIN_QML_TARGET_BASE_PATH=/usr/imports/Sharing/plugins/
}
TEMPLATE = lib
QT += dbus
CONFIG += plugin link_pkgconfig
INCLUDEPATH += ../../common ../../interfaces ../libsharingserviceplugin

#For some reason, moc doesn't pick up the cflags provided by pkgconfig.
#So, we add them by hand. I'm probably doing something wrong here - please
#let me know if you know the Right Way to do this - james.ausmus@intel.com

INCLUDEPATH += $$system(pkg-config --cflags libsharingserviceplugin \
| tr \' \' \'\\n\' | grep ^-I | cut -d 'I' -f 2-)

LIBS += -L../libsharingserviceplugin

PKGCONFIG += libsharingserviceplugin

HEADERS  = example.h \
    exampleservice.h

OTHER_FILES += qml/*.qml

SOURCES  = example.cpp \
    exampleservice.cpp

TARGET = example
target.path = $$SHARINGD_PLUGIN_PATH

qml.files = qml/*.qml
qml.path = $$PLUGIN_QML_TARGET_BASE_PATH/example/

INSTALLS += target qml
