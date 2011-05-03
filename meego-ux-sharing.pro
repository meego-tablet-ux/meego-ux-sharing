include(common.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += plugins
SUBDIRS += daemon
SUBDIRS += libmeegouxsharingclient
SUBDIRS += meego-ux-sharing-qml
SUBDIRS += meego-ux-sharing-qml-ui

OTHER_FILES += interfaces/*.xml

pcfile.path=/usr/lib/pkgconfig
pcfile.files=meego-ux-sharing.pc

common_headers.path = /usr/include/meego-ux-sharing
common_headers.files = common/meegouxsharingdbustypes.h \
    common/meegouxsharingcommon.h

INSTALLS += pcfile common_headers

dist.commands += rm -Rf $${DISTDIR} &&
dist.commands += git clone . $${DISTDIR} &&
dist.commands += rm -Rf $${DISTDIR}/.git &&
dist.commands += mkdir -p $${TSDIR} &&
dist.commands += cd daemon && make dist && cd .. &&
dist.commands += cd libmeegouxsharingclient && make dist && cd .. &&
dist.commands += cd meego-ux-sharing-qml && make dist && cd .. &&
dist.commands += cd plugins/libmeegouxsharingserviceplugin && make dist && cd ../.. &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}

QMAKE_EXTRA_TARGETS += dist
