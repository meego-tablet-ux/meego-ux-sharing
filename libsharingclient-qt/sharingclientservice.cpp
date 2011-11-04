/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "sharingclientservice.h"
#include "sharing-service_interface.h"
#include "sharing-daemon_interface.h"

#include <sharingcommon.h>

#define SHARINGSERVICEPATH QString("/com/intel/sharing/services/%1")


SharingClientService::SharingClientService(QString serviceName,
                                                     const QDBusConnection &conn,
                                                     QObject *parent) :
    QObject(parent),
    mConn(conn)
{
    if (serviceName.isEmpty())
        return;
    setServiceName(serviceName);
}


void SharingClientService::setServiceName(QString serviceName)
{
    mServiceIntf = new ComIntelSharingServiceInterface(
            ComIntelSharingInterface::staticInterfaceName(),
            SHARINGSERVICEPATH.arg(serviceName), mConn, this);

    connect(mServiceIntf,
            SIGNAL(ServiceChanged(QString)),
            this,
            SLOT(onServiceChanged(QString)));
    loadServiceData();

}

QString SharingClientService::getServiceName()
{
    return mServiceName;
}

QString SharingClientService::getDisplayName()
{
    return mDisplayName;
}

QString SharingClientService::getIconPath()
{
    return mIconPath;
}

QString SharingClientService::getServiceDesc()
{
    return mServiceDesc;
}

QString SharingClientService::getServiceType()
{
    return mServiceType;
}

uint SharingClientService::getCredsState()
{
    return mCredsState;
}

bool SharingClientService::getServiceAvailable()
{
    return mServiceAvail;
}

QString SharingClientService::getServiceStateText()
{
    return mServiceStateText;
}

bool SharingClientService::canShareType(const QString &sharetype)
{
    QDBusPendingReply<bool> reply = mServiceIntf->CanShareType(sharetype);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << QString("Error in CanShareType call for %1: %2").arg(mServiceName, reply.error().message());
        return false;
    }
    return reply.value();
}

QString SharingClientService::getSettingsURI(const QString &platform, const QString &product)
{
    QDBusPendingReply<QString> reply = mServiceIntf->GetSettingsURI(platform, product);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << QString("Error in getSettingsURI call for %1: %2").arg(mServiceName, reply.error().message());
        return QString();
    }
    return reply.value();
}

QString SharingClientService::getUIName(const QString &widgettype,
                                             const QString &platform,
                                             const QString &product,
                                             const QString &sharetype,
                                             uint sharecount)
{
    QDBusPendingReply<QString> reply = mServiceIntf->GetUIName(widgettype, platform, product, sharetype, sharecount);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << QString("Error in getUIName call for %1: %2").arg(mServiceName, reply.error().message());
        return QString();
    }
    return reply.value();
}

//Protected functions:

void SharingClientService::loadServiceData()
{
    QDBusPendingReply<QString> serviceNameReply = mServiceIntf->GetServiceName();
    serviceNameReply.waitForFinished();
    if (serviceNameReply.isError()) {
        qDebug() << QString("Couldn't get service name for %1: %2!").arg(mServiceName, serviceNameReply.error().message());
        mServiceName = QString();
    } else {
        mServiceName = serviceNameReply.value();
    }


    QDBusPendingReply<QString> displayNameReply = mServiceIntf->GetDisplayName();
    displayNameReply.waitForFinished();
    if (displayNameReply.isError()) {
        qDebug() << QString("Couldn't get display name for %1: %2!").arg(mServiceName, displayNameReply.error().message());
        mDisplayName = QString();
    } else {
        mDisplayName = displayNameReply.value();
    }


    QDBusPendingReply<QString> iconPathReply = mServiceIntf->GetIconPath();
    iconPathReply.waitForFinished();
    if (iconPathReply.isError()) {
        qDebug() << QString("Couldn't get icon path for %1: %2!").arg(mServiceName, iconPathReply.error().message());
        mIconPath = QString();
    } else {
        mIconPath = iconPathReply.value();
    }

    QDBusPendingReply<QString> serviceDescReply = mServiceIntf->GetServiceDesc();
    serviceDescReply.waitForFinished();
    if (serviceDescReply.isError()) {
        qDebug() << QString("Couldn't get service description for %1: %2!").arg(mServiceName, serviceDescReply.error().message());
        mServiceDesc = QString();
    } else {
        mServiceDesc = serviceDescReply.value();
    }

    QDBusPendingReply<QString> serviceTypeReply = mServiceIntf->GetServiceType();
    serviceTypeReply.waitForFinished();
    if (serviceTypeReply.isError()) {
        qDebug() << QString("Couldn't get service description for %1: %2!").arg(mServiceName, serviceTypeReply.error().message());
        mServiceType = QString();
    } else {
        mServiceType = serviceTypeReply.value();
    }

    QDBusPendingReply<uint> credsStateReply = mServiceIntf->GetCredsState();
    credsStateReply.waitForFinished();
    if (credsStateReply.isError()) {
        qDebug() << QString("Couldn't get creds state for %1: %2!").arg(mServiceName, credsStateReply.error().message());
        mCredsState = CredsStateUnknown;
    } else {
        mCredsState = credsStateReply.value();
    }

    QDBusPendingReply<bool> serviceAvailReply = mServiceIntf->GetServiceAvailable();
    serviceAvailReply.waitForFinished();
    if (serviceAvailReply.isError()) {
        qDebug() << QString("Couldn't get Service Available data for %1: %2!").arg(mServiceName, serviceAvailReply.error().message());
        mServiceAvail = false;
    } else {
        mServiceAvail = serviceAvailReply.value();
    }

    QDBusPendingReply<QString> serviceStateTextReply = mServiceIntf->GetServiceStateText();
    serviceStateTextReply.waitForFinished();
    if (serviceStateTextReply.isError()) {
        qDebug() << QString("Couldn't get Service State Text for %1: %2!").arg(mServiceName, serviceStateTextReply.error().message());
        mServiceStateText = QString();
    } else {
        mServiceStateText = serviceStateTextReply.value();
    }

}

void SharingClientService::onServiceChanged(const QString &service)
{
    if (service != mServiceName)
        return;
    loadServiceData();
    emit ServiceChanged(this);
}
