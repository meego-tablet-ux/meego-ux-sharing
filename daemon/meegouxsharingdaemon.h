/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGDAEMON_H
#define MEEGOUXSHARINGDAEMON_H

#include <QObject>
#include <QHash>
#include <QMutex>
#include <QStringList>
#include <meegouxsharingcommon.h>
#include <meegouxsharingdbustypes.h>

class QPluginLoader;
class SharingAdaptor; //Generated DBUS adaptor class
class MeeGoUXSharingServicePluginInterface;

class MeeGoUXSharingDaemon : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoUXSharingDaemon(QObject *parent = 0);
    ~MeeGoUXSharingDaemon();
    bool init();

    int Share(const QString &service, const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage);
    bool CancelShare(int opid);

signals:
    void ServiceAdded(const QString &name, const QString &type);
    void ServiceRemoved(const QString &name, const QString &type);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

public slots:
    QStringList GetAllServices();
    QStringList GetServicesForType(const QString &type);
    void RescanPlugins();

private slots:
    void onSvcShareProgress(const QString &service, int opid, int progress, const QString &message);

private:
    bool loadPlugins();
    bool unloadPlugins();

    QHash<QString, QPluginLoader *> mPlugins;
    //mServices = <ServiceName, ShareType>
    QList<MeeGoUXSharingServiceInfo> mServices;  //1 Plugin can provide > 1 service, and each service can service >= 1 type
    QHash<QString, MeeGoUXSharingServicePluginInterface *> mServiceToPlugin;
    QMap<int, QPair<QString, int> > mGlobalShareOpToService;
    SharingAdaptor *mAdaptor;
    QMutex mPluginMutex;
    int mGlobalOpID;
    bool mClosing;
};

#endif // MEEGOUXSHARINGDAEMON_H
