/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGSERVICEPLUGININTERFACE_H
#define SHARINGSERVICEPLUGININTERFACE_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QtPlugin>
#include <QMetaType>
#include <sharingcommon.h>
#include <sharingdbustypes.h>

const QString SHARING_SERVICE_PLUGIN_VERSION = "version";
const QString SHARING_SERVICE_PLUGIN_NAME    = "name";
const QString SHARING_SERVICE_PLUGIN_DESC    = "comment";

class SharingServicePluginInterface: public QObject
{
    Q_OBJECT
public:
    explicit SharingServicePluginInterface(QObject *parent = 0): QObject(parent) {}
    virtual ~SharingServicePluginInterface() { }
    virtual bool init() = 0;
    virtual QHash<QString, QString> metaData() = 0;
    virtual QList<SharingServiceInfo> getServices() = 0;
    virtual int Share(QString serviceName, const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage) = 0;
    virtual bool CancelShare(QString serviceName, int opid) = 0;

signals:
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

};

Q_DECLARE_INTERFACE(SharingServicePluginInterface, "com.intel.sharing.Plugin/0.1")

#endif //SHARINGSERVICEPLUGININTERFACE_H

