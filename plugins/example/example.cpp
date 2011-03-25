/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "example.h"
#include "meegouxsharingdbustypes.h"

#include <QDebug>

Example::Example(QObject *parent):
        MeeGoUXSharingServicePluginInterface(parent)
{
}

Example::~Example()
{
}

bool Example::init()
{
    qDebug() << "Example Plugin initialization";
    MeeGoUXSharingServiceInfo foo;
    foo.serviceName = "foo";
    foo.serviceTypes << MEEGO_SHARE_TYPE_IMAGE;
    mServiceInfoList << foo;

    MeeGoUXSharingServiceInfo bar;
    bar.serviceName = "bar";
    bar.serviceTypes << MEEGO_SHARE_TYPE_IMAGE << MEEGO_SHARE_TYPE_VIDEO;
    mServiceInfoList << bar;

    foreach (MeeGoUXSharingServiceInfo info, mServiceInfoList) {
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
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_NAME]    = QString("example");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_VERSION] = QString("0.0.2");
    info[MEEGO_UX_SHARING_SERVICE_PLUGIN_DESC]    = QString("Example plugin for meego-ux-sharing");
    return info;
}

QList<MeeGoUXSharingServiceInfo> Example::getServices()
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

