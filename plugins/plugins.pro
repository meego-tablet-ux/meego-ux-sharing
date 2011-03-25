include(../common.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += libmeegouxsharingserviceplugin
#SUBDIRS += example

QMAKE_EXTRA_TARGETS += dist
