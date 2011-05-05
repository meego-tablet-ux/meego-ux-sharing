/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGSERVICECOMMON_H
#define MEEGOUXSHARINGSERVICECOMMON_H

#include <QString>
#include <QStringList>
#include <QMetaType>

const QString MEEGO_SHARE_TYPE_IMAGE = "x-meego-ux-sharing/image";
const QString MEEGO_SHARE_TYPE_VIDEO = "x-meego-ux-sharing/video";
const QString MEEGO_SHARE_TYPE_AUDIO = "x-meego-ux-sharing/audio";
const QString MEEGO_SHARE_TYPE_TEXT  = "x-meego-ux-sharing/text";
const QString MEEGO_SHARE_TYPE_VCARD  = "x-meego-ux-sharing/vcard";


struct MeeGoUXSharingServiceInfo {
    QString serviceName;
    QStringList serviceTypes;
};

enum MeeGoUXSharingCredentialsState {
    CredsStateValid = 0,
    CredsStateInvalid = 1,
    CredsStateUnknown = 2
};

enum MeeGoUXSharingType {
    ShareTypeImage = 0,
    ShareTypeVideo = 1,
    ShareTypeAudio = 2,
    ShareTypeText = 3,
    ShareTypeVCard = 4,
};

Q_DECLARE_METATYPE(MeeGoUXSharingServiceInfo)
Q_ENUMS(MeeGoUXSharingType)

#endif //MEEGOUXSHARINGSERVICECOMMON_H
