include(../common.pri)
TARGET = UI
TEMPLATE = subdirs

qmldir.files += $$TARGET
qmldir.path += $$[QT_INSTALL_IMPORTS]/MeeGo/Sharing/
INSTALLS += qmldir

PKG_NAME = meego-ux-sharing-qml-ui
TRANSLATIONS += $${HEADERS} \
        $${SOURCES}
dist.commands += lupdate $${TRANSLATIONS} -ts $${TSDIR}/$${PKG_NAME}.ts

QMAKE_EXTRA_TARGETS += dist

