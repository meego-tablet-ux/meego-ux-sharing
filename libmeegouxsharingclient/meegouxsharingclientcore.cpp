/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingclientcore.h"
#include <meego-ux-sharing-daemon_interface.h>
#include "meegouxsharingclientservice.h"

#define MEEGOUXSHARINGDAEMONPATH "/com/meego/ux/sharing"

MeeGoUXSharingClientCore::MeeGoUXSharingClientCore(const QDBusConnection &connection,
                                               QObject *parent) :
    QObject(parent),
    mConnection(connection)
{
    mDaemonIntf = new ComMeegoUxSharingInterface(
            ComMeegoUxSharingInterface::staticInterfaceName(),
            MEEGOUXSHARINGDAEMONPATH, connection, this);

//    connect(mDaemonIntf,
//            SIGNAL(ServiceAdded(QString,QString)),
//            this,
//            SLOT(onServiceAdded(QString, QString)));

//    connect(mDaemonIntf,
//            SIGNAL(ServiceRemoved(QString,QString)),
//            this,
//            SLOT(onServiceRemoved(QString,QString)));

    connect(mDaemonIntf,
            SIGNAL(ServiceAdded(QString,QString)),
            this,
            SIGNAL(ServiceAdded(QString,QString)));

    connect(mDaemonIntf,
            SIGNAL(ServiceRemoved(QString,QString)),
            this,
            SIGNAL(ServiceRemoved(QString,QString)));

}


QStringList MeeGoUXSharingClientCore::getServicesForSharingType(QString sharingType)
{
    QDBusPendingReply<QStringList> reply = mDaemonIntf->GetServicesForType(sharingType);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << QString("Couldn't get list of services for type %1: %2").arg(sharingType, reply.error().message());
        return QStringList();
    }

    mServicesList = reply.value();
    return mServicesList;
}

QStringList MeeGoUXSharingClientCore::getServiceTypes(QString sharingType)
{
    QStringList serviceTypes;
    getServicesForSharingType(sharingType);
//    qDebug() << QString("In MSCC::getServiceTypes(%1) - services:").arg(sharingType);
    qDebug() << mServicesList;
    foreach (QString service, mServicesList) {
        MeeGoUXSharingClientService *clientService = new MeeGoUXSharingClientService(service,
                                                                                 getConnection());
        if (!clientService)
            continue;
        if (!serviceTypes.contains(clientService->getServiceType()))
            serviceTypes.append(clientService->getServiceType());
    }
    return serviceTypes;
}

bool MeeGoUXSharingClientCore::cancelShare(int opid)
{
    QDBusPendingReply<bool> reply = mDaemonIntf->CancelShare(opid);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug()<< QString("Couldn't cancel share opID %1: %2!").arg(QString::number(opid), reply.error().message());
        return false;
    }
    return reply.value();
}

int MeeGoUXSharingClientCore::share(QString serviceName,
                                  QString sharingType,
                                  ArrayOfShareItemStruct items,
                                  QString &errMessage)
{
    QDBusPendingReply<int, QString> reply = mDaemonIntf->Share(serviceName, sharingType, items);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << QString("Couldn't perform share on service %1: %2!").arg(serviceName, reply.error().message());
        errMessage = reply.error().message();
        return -1;
    }
    errMessage = reply.argumentAt(1).toString();
    return reply.argumentAt(0).toInt();
}


const QDBusConnection & MeeGoUXSharingClientCore::getConnection() const
{
    return mConnection;
}
