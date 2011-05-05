/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingclient.h"
#include "meegouxsharingclientservicemodel.h"
#include "meegouxsharingclientcore.h"

#include "meegouxsharingcommon.h"
#include <meegouxsharingdbustypes.h>

QString MeeGoUXSharingClient::TranslateShareType(MeeGoUXSharingType type)
{
    switch (type) {
    case ShareTypeImage:
        return MEEGO_SHARE_TYPE_IMAGE;
        break;
    case ShareTypeVideo:
        return MEEGO_SHARE_TYPE_VIDEO;
        break;
    case ShareTypeAudio:
        return MEEGO_SHARE_TYPE_AUDIO;
        break;
    case ShareTypeText:
        return MEEGO_SHARE_TYPE_TEXT;
        break;
    case ShareTypeVCard:
        return MEEGO_SHARE_TYPE_VCARD;
        break;
    default:
        return QString();
        break;
    }
}

MeeGoUXSharingClient::MeeGoUXSharingClient(const QDBusConnection &connection,
                                       QObject *parent):
        QObject(parent),
        mCore(new MeeGoUXSharingClientCore(connection, this))
{
    registerMeeGoUXSharingDBUSDataTypes();
}

QStringList MeeGoUXSharingClient::getServiceTypes(MeeGoUXSharingType type)
{
    QString stype = MeeGoUXSharingClient::TranslateShareType(type);
//    qDebug() << QString("MSC::getServiceTypes, MST Type %1, stype: %2").arg(QString::number((uint)type), stype);
    if (stype.isEmpty())
        return QStringList();
//    qDebug() << QString("stype is not empty!");
    return getServiceTypesCustom(stype);
}

QStringList MeeGoUXSharingClient::getServiceTypesCustom(QString customType)
{
    return mCore->getServiceTypes(customType);
}

MeeGoUXSharingClientServiceModel * MeeGoUXSharingClient::getServiceModel(QString serviceType, MeeGoUXSharingType type)
{
    QString stype = TranslateShareType(type);
    if (stype.isEmpty())
        return 0;
    return getServiceModelCustom(serviceType, stype);
}

MeeGoUXSharingClientServiceModel * MeeGoUXSharingClient::getServiceModelCustom(QString serviceType, QString customType)
{
    return new MeeGoUXSharingClientServiceModel(serviceType, customType, mCore, this);
}

bool MeeGoUXSharingClient::cancelShare(int opid)
{
    if (!mShareItemStatuses.contains(opid))
        return true;
    if (mCore->cancelShare(opid)) {
        mShareItemStatuses.remove(opid);
        return true;
    }
    return false;
}

int MeeGoUXSharingClient::shareSimple(QString serviceName, QString sharingType, ArrayOfShareItemStruct items)
{
    QString errMsg;
    int opid = share(serviceName, sharingType, items, errMsg);
    if (opid == -1)
        mLastShareError = errMsg;
    return opid;
}

int MeeGoUXSharingClient::share(QString serviceName, QString sharingType, ArrayOfShareItemStruct items, QString &errmessage)
{
    int opid = mCore->share(serviceName, sharingType, items, errmessage);
    if (opid != -1) {
        if (mShareItemStatuses.contains(opid)) {
            qDebug() << QString("Error! Duplicate operation UID detected for opid %1 - replacing!").arg(opid);
            mShareItemStatuses.remove(opid);
        }
        mShareItemStatuses.insert(opid, 0);
    }
    return opid;
}

QString MeeGoUXSharingClient::getLastShareError()
{
    return mLastShareError;
}

//Protected slots:

void MeeGoUXSharingClient::onShareProgress(const QString &service, int opid,
                                                int progress, const QString &message)
{
    //If it's not our item that was shared, don't bother emitting signals for it...
    if (!mShareItemStatuses.contains(opid))
        return;
    //If we've errored out or completed, remove from QMap
    if ((progress == -1) || (progress == 100))
        mShareItemStatuses.remove(opid);
    emit ShareProgress(service, opid, progress, message);
}

