/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "sharingservice.h"

#define DBUSINTFNAME QString("com.intel.sharing")
#define DBUSOBJPATH QString("/com/intel/sharing/services/%1")

SharingService::SharingService(const SharingServiceInfo &serviceInfo,
                                         QObject *parent):
        QObject(parent),
        mServiceInfo(serviceInfo)
{
    registerSharingDBUSDataTypes();
    mServiceAdaptor = new ServiceAdaptor(this);

    QDBusConnection::sessionBus().registerObject(GetDBusObjPath(), this);
    QDBusConnection::sessionBus().registerService(DBUSINTFNAME);
}

SharingService::~SharingService()
{
    QDBusConnection::sessionBus().unregisterObject(GetDBusObjPath());
}

QString SharingService::GetDBusObjPath() const
{
    return DBUSOBJPATH.arg(mServiceInfo.serviceName);
}
