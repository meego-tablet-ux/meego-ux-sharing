/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "example.h"
#include "sharingdbustypes.h"

#include <QDebug>

Example::Example(QObject *parent):
        SharingServicePluginInterface(parent)
{
}

Example::~Example()
{
}

bool Example::init()
{
    qDebug() << "Example Plugin initialization";
    SharingServiceInfo foo;
    foo.serviceName = "foo";
    foo.serviceTypes << SHARE_TYPE_IMAGE;
    mServiceInfoList << foo;

    SharingServiceInfo bar;
    bar.serviceName = "bar";
    bar.serviceTypes << SHARE_TYPE_IMAGE << SHARE_TYPE_VIDEO;
    mServiceInfoList << bar;

    foreach (SharingServiceInfo info, mServiceInfoList) {
        ExampleService *svc = new ExampleService(info, this);
        connect(svc,
                SIGNAL(ShareProgress(QString,int,int,QString)),
                this,
                SIGNAL(ShareProgress(QString,int,int,QString)));
        mServices.append(svc);
    }
    return true;
}

QHash<QString, QString> Example::metaData()
{
    QHash<QString, QString> info;
    info[SHARING_SERVICE_PLUGIN_NAME]    = QString("example");
    info[SHARING_SERVICE_PLUGIN_VERSION] = QString("0.1.0");
    info[SHARING_SERVICE_PLUGIN_DESC]    = QString("Example plugin for sharing");
    return info;
}

QList<SharingServiceInfo> Example::getServices()
{
    return mServiceInfoList;
}

int Example::Share(QString serviceName, const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage)
{
    foreach (ExampleService *svc, mServices) {
        if (svc->GetServiceName() == serviceName)
            return svc->Share(sharetype, items, errmessage);
    }
    errmessage = QString("Invalid service %1 specified!").arg(serviceName);
    return -1;
}

bool Example::CancelShare(QString serviceName, int opid)
{
    foreach (ExampleService *svc, mServices) {
        if (svc->GetServiceName() == serviceName)
            return svc->CancelShare(opid);
    }
    qWarning() << QString("Invalid service %1 specified in CancelShare for opid %2!").arg(serviceName, QString::number(opid));
    return false;
}




Q_EXPORT_PLUGIN2(example, Example);

