include(common.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += plugins
SUBDIRS += daemon
SUBDIRS += libsharingclient-qt
SUBDIRS += libsharingclient-glib
SUBDIRS += libsharingclient-qml
SUBDIRS += libsharingclient-qml-ui

OTHER_FILES += interfaces/*.xml

pcfile.path=/usr/lib/pkgconfig
pcfile.files=sharing.pc

common_headers.path = /usr/include/sharing
common_headers.files = common/sharingdbustypes.h \
    common/sharingcommon.h

INSTALLS += pcfile common_headers

dist.commands += rm -Rf $${DISTDIR} &&
dist.commands += git clone . $${DISTDIR} &&
dist.commands += rm -Rf $${DISTDIR}/.git &&
dist.commands += mkdir -p $${TSDIR} &&
dist.commands += cd daemon && make dist && cd .. &&
dist.commands += cd libsharingclient-qt && make dist && cd .. &&
dist.commands += cd libsharingclient-qml && make dist && cd .. &&
dist.commands += cd libsharingclient-qml-ui && make dist && cd .. &&
dist.commands += cd libsharingclient-glib && make dist && cd .. &&
dist.commands += cd plugins/libsharingserviceplugin && make dist && cd ../.. &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}

QMAKE_EXTRA_TARGETS += dist
