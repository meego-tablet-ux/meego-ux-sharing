/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGCOMMON_H
#define SHARINGCOMMON_H

#include <QString>
#include <QStringList>
#include <QMetaType>

const QString SHARE_TYPE_IMAGE = "image/*";
const QString SHARE_TYPE_VIDEO = "video/*";
const QString SHARE_TYPE_AUDIO = "audio/*";
const QString SHARE_TYPE_TEXT  = "text/*";
const QString SHARE_TYPE_VCARD  = "text/vcard";


struct SharingServiceInfo {
    QString serviceName;
    QStringList serviceTypes;
};

enum SharingCredentialsState {
    CredsStateValid = 0,
    CredsStateInvalid = 1,
    CredsStateUnknown = 2
};

enum SharingType {
    ShareTypeImage = 0,
    ShareTypeVideo = 1,
    ShareTypeAudio = 2,
    ShareTypeText = 3,
    ShareTypeVCard = 4,
};

Q_DECLARE_METATYPE(SharingServiceInfo)
Q_ENUMS(SharingType)

#endif //SHARINGCOMMON_H
