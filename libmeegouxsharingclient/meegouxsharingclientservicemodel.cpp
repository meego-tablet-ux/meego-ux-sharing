/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingclientservicemodel.h"
#include "meegouxsharingclientcore.h"
#include "meegouxsharingclientservice.h"
#include <meegouxsharingcommon.h>

enum SharingDataRoles {
    SharingRoleName        = Qt::UserRole +  0,  //QString
    SharingRoleDisplayName = Qt::UserRole +  1,  //QString
    SharingRoleIconPath    = Qt::UserRole +  2,  //QUrl
    SharingRoleDesc        = Qt::UserRole +  3,  //QString
    SharingRoleServiceType = Qt::UserRole +  4,  //QString
    SharingRoleCredsState  = Qt::UserRole +  5,  //uint
    SharingRoleAvailable   = Qt::UserRole +  6,  //bool
    SharingRoleStateText   = Qt::UserRole +  7,  //QString
    SharingRoleSettingsURI = Qt::UserRole +  8,  //QString
    SharingRoleUIName      = Qt::UserRole +  9,  //QString
    SharingRoleServiceObj  = Qt::UserRole + 10,  //MeeGoUXSharingClientService *
};

MeeGoUXSharingClientServiceModel::MeeGoUXSharingClientServiceModel(QString serviceType,
                                                   QString shareType,
                                                   MeeGoUXSharingClientCore *core,
                                                   QObject *parent) :
    QAbstractListModel(parent),
    mCore(core),
    mServiceType(serviceType),
    mShareType(shareType),
    mInitialized(false)
{
    QHash<int, QByteArray> roles;
    roles.insert(SharingRoleName, "name");
    roles.insert(SharingRoleDisplayName, "displayName");
    roles.insert(SharingRoleIconPath, "icon");
    roles.insert(SharingRoleDesc, "desc");
    roles.insert(SharingRoleServiceType, "type");
    roles.insert(SharingRoleCredsState, "credsState");
    roles.insert(SharingRoleAvailable, "avail");
    roles.insert(SharingRoleStateText, "stateText");
    roles.insert(SharingRoleSettingsURI, "settings");
    roles.insert(SharingRoleUIName, "uiname");
    roles.insert(SharingRoleServiceObj, "serviceObj");
    setRoleNames(roles);

    if (!mCore)
        mCore = new MeeGoUXSharingClientCore();

    initialize();
    filter();
}

void MeeGoUXSharingClientServiceModel::setShareType(QString shareType)
{
    mShareType = shareType;
    if (!mInitialized)
        initialize();
    filter();
}

void MeeGoUXSharingClientServiceModel::setServiceType(QString serviceType)
{
    mServiceType = serviceType;
    if (!mInitialized)
        initialize();
    filter();
}

void MeeGoUXSharingClientServiceModel::initialize()   //Private
{
    if (mServiceType.isEmpty() || mShareType.isEmpty() || !mCore) {
        return;
    }
    mInitialized = true;

    connect(mCore,
            SIGNAL(ServiceAdded(QString,QString)),
            this,
            SLOT(onServiceAdded(QString, QString)));
    connect(mCore,
            SIGNAL(ServiceRemoved(QString,QString)),
            this,
            SLOT(onServiceRemoved(QString, QString)));
}

void MeeGoUXSharingClientServiceModel::filter()   //Private
{
    QStringList serviceList = mCore->getServicesForSharingType(mShareType);
    foreach(QString service, serviceList) {
        MeeGoUXSharingClientService *clientService = new MeeGoUXSharingClientService(service,
                                                                                 mCore->getConnection(),
                                                                                 this);
        if (clientService->getServiceType() == mServiceType
            && clientService->getCredsState() == CredsStateValid) {
            mServiceList.append(service);
            connect(clientService,
                    SIGNAL(ServiceChanged(MeeGoUXSharingClientService*)),
                    this,
                    SLOT(onServiceChanged(MeeGoUXSharingClientService *)));
/*            connect(clientService,
                    SIGNAL(ShareProgress(MeeGoUXSharingClientService*,int,int,QString)),
                    this,
                    SIGNAL(ShareProgress(MeeGoUXSharingClientService*,int,int,QString)));*/
            emit this->beginInsertRows(QModelIndex(), mServices.count(), mServices.count());
            mServices.append(clientService);
            emit this->endInsertRows();
        }
    }

}

int MeeGoUXSharingClientServiceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mServices.count();
}

QVariant MeeGoUXSharingClientServiceModel::data(const QModelIndex &index, int role) const
{
    MeeGoUXSharingClientService *service = mServices.at(index.row());
    if (!service)
        return QVariant();

    switch (role) {
    case SharingRoleName:
        return QVariant::fromValue<QString>(service->getServiceName());
        break;
    case SharingRoleDisplayName:
        return QVariant::fromValue<QString>(service->getDisplayName());
        break;
    case SharingRoleIconPath:
        return QVariant::fromValue<QString>(service->getIconPath());
        break;
    case SharingRoleDesc:
        return QVariant::fromValue<QString>(service->getServiceDesc());
        break;
    case SharingRoleServiceType:
        return QVariant::fromValue<QString>(service->getServiceType());
        break;
    case SharingRoleCredsState:
        return QVariant::fromValue<uint>(service->getCredsState());
        break;
    case SharingRoleAvailable:
        return QVariant::fromValue<bool>(service->getServiceAvailable());
        break;
    case SharingRoleStateText:
        return QVariant::fromValue<QString>(service->getServiceStateText());
        break;
    case SharingRoleServiceObj:
        return QVariant::fromValue<MeeGoUXSharingClientService *>(service);

    }
    return QVariant();

}

QString MeeGoUXSharingClientServiceModel::getServiceNameFromDisplayName(QString displayName)
{
    foreach (MeeGoUXSharingClientService *svc, mServices) {
        if (svc->getDisplayName() == displayName)
            return svc->getServiceName();
    }
    return QString();
}

QStringList MeeGoUXSharingClientServiceModel::getServiceTypesList()
{
    QStringList svcs;
    foreach (MeeGoUXSharingClientService *svc, mServices)
        svcs.append(svc->getDisplayName());
    svcs.sort();
    return svcs;
}

//Protected slots:

void MeeGoUXSharingClientServiceModel::onServiceChanged(MeeGoUXSharingClientService *svc)
{
    int i;
    for (i = 0; i < mServices.count(); ++i) {
        if (svc == mServices.at(i)) {
            QModelIndex qmi = this->createIndex(i, 0, 0);
            emit this->dataChanged(qmi, qmi);
            break;
        }
    }
}

void MeeGoUXSharingClientServiceModel::onServiceAdded(QString name, QString shareType)
{
    if ((shareType == this->mShareType) && (!mServiceList.contains(name))) {
        MeeGoUXSharingClientService *clientService = new MeeGoUXSharingClientService(name, mCore->getConnection(), this);
        if (clientService->getServiceType() == mServiceType) {
            connect(clientService,
                    SIGNAL(ServiceChanged(MeeGoUXSharingClientService*)),
                    this,
                    SLOT(onServiceChanged(MeeGoUXSharingClientService *)));
            connect(clientService,
                    SIGNAL(ShareProgress(MeeGoUXSharingClientService*,int,int,QString)),
                    this,
                    SIGNAL(ShareProgress(MeeGoUXSharingClientService*,int,int,QString)));
            emit this->beginInsertRows(QModelIndex(), mServices.count(), mServices.count());
            mServices.append(clientService);
            emit this->endInsertRows();
        }
    }
}

void MeeGoUXSharingClientServiceModel::onServiceRemoved(QString name, QString shareType)
{
    if ((shareType == this->mShareType) && (mServiceList.contains(name))) {
        mServiceList.removeAll(name);
        int i;
        for (i = 0; i < mServices.count(); ++i) {
            if (mServices.at(i)->getServiceName() == name) {
                mServices.at(i)->deleteLater();
                mServices.removeAt(i);
            }
        }
    }
}
