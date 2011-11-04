include(../common.pri)
TARGET = UI
TEMPLATE = subdirs

qmldir.files += $$TARGET
qmldir.path += $$[QT_INSTALL_IMPORTS]/Sharing/
INSTALLS += qmldir

OTHER_FILES += sharetest.qml

PKG_NAME = libsharingclient-qml-ui
TRANSLATIONS += $${HEADERS} \
        $${SOURCES} \
        $${TARGET}/*.qml
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

