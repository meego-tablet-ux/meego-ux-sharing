include(../common.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += libsharingserviceplugin
#SUBDIRS += example

QMAKE_EXTRA_TARGETS += dist
