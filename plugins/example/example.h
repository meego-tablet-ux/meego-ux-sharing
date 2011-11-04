/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <sharingserviceplugininterface.h>
#include <QObject>
#include <QStringList>
#include <sharingservice.h>

#include "exampleservice.h"

class Example : public SharingServicePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(SharingServicePluginInterface)

public:
    Example(QObject *parent = 0);
    ~Example();
    bool init();
    QHash<QString, QString> metaData();
    QList<SharingServiceInfo> getServices();

    int Share(QString serviceName, const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage);
    bool CancelShare(QString serviceName, int opid);

signals:
    void ServiceChanged(QString serviceName);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

private:
    QList<SharingServiceInfo> mServiceInfoList;
    QList<ExampleService *> mServices;

};
