/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingservice.h"

#define DBUSINTFNAME QString("com.meego.ux.sharing")
#define DBUSOBJPATH QString("/com/meego/ux/sharing/services/%1")

MeeGoUXSharingService::MeeGoUXSharingService(const MeeGoUXSharingServiceInfo &serviceInfo,
                                         QObject *parent):
        QObject(parent),
        mServiceInfo(serviceInfo)
{
    registerMeeGoUXSharingDBUSDataTypes();
    mServiceAdaptor = new ServiceAdaptor(this);

    QDBusConnection::sessionBus().registerObject(GetDBusObjPath(), this);
    QDBusConnection::sessionBus().registerService(DBUSINTFNAME);
}

MeeGoUXSharingService::~MeeGoUXSharingService()
{
    QDBusConnection::sessionBus().unregisterObject(GetDBusObjPath());
}

QString MeeGoUXSharingService::GetDBusObjPath() const
{
    return DBUSOBJPATH.arg(mServiceInfo.serviceName);
}
