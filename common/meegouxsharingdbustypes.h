/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGSERVICEDBUSTYPES_H
#define MEEGOUXSHARINGSERVICEDBUSTYPES_H

#include <QMetaType>
#include <QList>
#include <QHash>
#include <QString>
#include <QtDBus>

typedef QHash<QString,QString> ShareParams;


struct ShareItemStruct {
    QString shareURI;
    ShareParams params;
};

typedef QList<ShareItemStruct> ArrayOfShareItemStruct;


QDBusArgument &operator<<(QDBusArgument &arg, const ShareItemStruct &item);
const QDBusArgument &operator>>(const QDBusArgument &arg, ShareItemStruct &item);

inline void registerMeeGoUXSharingDBUSDataTypes() {
    qDBusRegisterMetaType<ShareItemStruct>();
    qDBusRegisterMetaType<ArrayOfShareItemStruct>();
//    qDBusRegisterMetaType<ShareParams>();
}

//Q_DECLARE_METATYPE(ShareParams);
Q_DECLARE_METATYPE(ShareItemStruct);
Q_DECLARE_METATYPE(ArrayOfShareItemStruct);

#endif // MEEGOUXSHARINGSERVICEDBUSTYPES_H
