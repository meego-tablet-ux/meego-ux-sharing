/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingdaemon.h"
#include "meegouxsharingserviceplugininterface.h"
#include <meego-ux-sharing-daemon_adaptor.h>

#include <QDir>
#include <QPluginLoader>
#include <QDebug>
#include <QMap>
#include <QPair>


#define DBUSINTFNAME QString("com.meego.ux.sharing")
#define DBUSOBJPATH QString("/com/meego/ux/sharing")


MeeGoUXSharingDaemon::MeeGoUXSharingDaemon(QObject *parent) :
    QObject(parent),
    mGlobalOpID(0),
    mClosing(false)
{
    mAdaptor = new SharingAdaptor(this);
    QDBusConnection::sessionBus().registerObject(DBUSOBJPATH, this);
    QDBusConnection::sessionBus().registerService(DBUSINTFNAME);

    //We also register at the system bus so we can get RescanPlugin
    //signals from an rpm install script that's not connected
    //to the user's session bus... All actual services are only
    //available on the session bus, so you can't do anything from the
    //system level other than get a list of plugins and rescan the plugins...

    //TODO - think through this a bit more...
    //QDBusConnection::systemBus().registerObject(DBUSOBJPATH, this);
    //QDBusConnection::systemBus().registerService(DBUSINTFNAME);
}

MeeGoUXSharingDaemon::~MeeGoUXSharingDaemon()
{
    //Mark as closing, so we don't reload plugins
    //if we happen to get a rescanPlugins DBUS call
    mClosing = true;
    unloadPlugins();
}

bool MeeGoUXSharingDaemon::init()
{
    //Load plugins
    return loadPlugins();
}

int MeeGoUXSharingDaemon::Share(const QString &service, const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage)
{
    //TODO-JEA
    MeeGoUXSharingServicePluginInterface *intf = mServiceToPlugin.value(service);
    if (!intf) {
        errmessage = QString("No service %1 found!").arg(service);
        return -1;
    }
    //mGlobalShareOpToService
    int opid = intf->Share(service, sharetype, items, errmessage);
    if (opid == -1)
        return opid;
    mGlobalShareOpToService.insert(mGlobalOpID, QPair<QString, int>(service, opid));
    return mGlobalOpID++;
    //Increment after we return the current value.
}

bool MeeGoUXSharingDaemon::CancelShare(int opid)
{
    //If we don't have an op with that ID, we just
    //pretend like we cancelled successfully...
    if (!mGlobalShareOpToService.contains(opid))
        return true;
    QPair <QString, int> serviceOp = mGlobalShareOpToService.value(opid);
    QString svcName = serviceOp.first;
    int svcop = serviceOp.second;
    MeeGoUXSharingServicePluginInterface *intf = mServiceToPlugin.value(svcName);
    if (!intf) {
        qWarning() << QString("Operation %1 found with invalid service name %2! Removing...").arg(QString::number(opid), svcName);
        return true;
    }
    if (intf->CancelShare(svcName, svcop)) {
        mGlobalShareOpToService.remove(opid);
        return true;
    }
    return false;
}

//Public slots:
QStringList MeeGoUXSharingDaemon::GetAllServices()
{
    QStringList services;
    foreach (MeeGoUXSharingServiceInfo info, mServices) {
        services << info.serviceName;
    }
    return services;
}

QStringList MeeGoUXSharingDaemon::GetServicesForType(const QString &type)
{
    QStringList services;
    foreach(MeeGoUXSharingServiceInfo info, mServices) {
        if (info.serviceTypes.contains(type))
            services << info.serviceName;
    }
    return services;
}

void MeeGoUXSharingDaemon::RescanPlugins()
{
    //This needs to do a load into a separate hash, then compare hashes and plugin metadata values
    //If we get an add, we emit added, if we get a delete, we emit a deleted, if a plugin changes
    //versions or any value, we do a deleted/added chain...
    //However, for now, we just do a unload/load, as long as we're not in the middle of closing...
    if (mClosing)
        return;
    unloadPlugins();
    loadPlugins();
}

//Private slots:

void MeeGoUXSharingDaemon::onSvcShareProgress(const QString &service, int opid,
                                            int progress, const QString &message)
{
    int globID = -1;

    QMap<int, QPair<QString, int> >::iterator i;
    for (i = mGlobalShareOpToService.begin();
         i != mGlobalShareOpToService.end(); ++i) {

        if ((i.value().first == service) && (i.value().second == opid)) {
            globID = i.key();
            break;
        }
    }
    if (globID == -1) {
        qWarning() <<
            QString("Got invalid opid (%1) in share progress signal from service %2 - progress %3, message %4!").arg(
                          QString::number(opid), service, QString::number(progress), message);
        return;
    }

    if ((progress == -1) || (progress == 100)) {
        mGlobalShareOpToService.remove(globID);
    }
    emit this->ShareProgress(service, globID, progress, message);
}

//Private functions:

bool MeeGoUXSharingDaemon::loadPlugins()
{
    //If we're currently closing, then don't try to load anything.
    //If we're not, try to lock the mutex, wait 3 seconds for it to
    //become available, and fail if we can't get it.
    if (mClosing || !mPluginMutex.tryLock(3000))
        return false;
    QDir dir = QDir(MSD_PLUGIN_PATH);
    QPluginLoader *plugin = 0;
    foreach (QString file, dir.entryList(QStringList() << QString("*.so"))) {
        plugin = new QPluginLoader(dir.absoluteFilePath(file), this);
        if (!plugin->load()) {
            qDebug() << QString("Couldn't load plugin %1: %2").arg(file, plugin->errorString());
            continue;
        }

        MeeGoUXSharingServicePluginInterface *intf = qobject_cast<MeeGoUXSharingServicePluginInterface *>(plugin->instance());
        if (!intf) {
            qDebug() << QString("Couldn't get MSSPI from plugin %1!").arg(file);
            plugin->unload();
            plugin->deleteLater();
            plugin = 0;
            continue;
        }
        if (!intf->init()) {
            qDebug() << QString("init() for plugin %1 failed! Skipping...").arg(file);
            plugin->unload();
            plugin->deleteLater();
            plugin = 0;
            continue;
        }

        QHash<QString, QString> props = intf->metaData();
        QString name = props.value(MEEGO_UX_SHARING_SERVICE_PLUGIN_NAME, QString());
        if (name.isEmpty()) {
            qDebug() << QString("Couldn't get plugin name from %1!").arg(file);
            plugin->unload();
            plugin->deleteLater();
            plugin = 0;
            continue;
        }
        qDebug() << QString("Successfully loaded plugin %1 from file %2!").arg(name, file);
        mPlugins.insert(name, plugin);
        foreach(MeeGoUXSharingServiceInfo info, intf->getServices()) {
            mServiceToPlugin.insert(info.serviceName, intf);
            mServices.append(info);
            foreach(QString type, info.serviceTypes) {
                emit ServiceAdded(info.serviceName, type);
            }
        }
        connect(intf,
                SIGNAL(ShareProgress(QString,int,int,QString)),
                this,
                SLOT(onSvcShareProgress(QString,int,int,QString)));
    }
    mPluginMutex.unlock();
    return true;
}

bool MeeGoUXSharingDaemon::unloadPlugins()
{
    mPluginMutex.lock();
    QHash<QString, QPluginLoader *>::iterator it = mPlugins.begin();
    while (it != mPlugins.end()) {
        QString name = it.key();
        qDebug() << QString("Unloading plugin %1").arg(name);
        QPluginLoader *plugin = it.value();

        if (plugin) {
            MeeGoUXSharingServicePluginInterface *intf =
                    qobject_cast<MeeGoUXSharingServicePluginInterface *>(plugin->instance());
            if (!intf) {
                qDebug() << QString("Ahh! Couldn't get an MSSPI from plugin %1 on unload! Abort, abort!").arg(it.key());
            } else {
                foreach(MeeGoUXSharingServiceInfo info, intf->getServices()) {
                    foreach(QString type, info.serviceTypes) {
                        emit ServiceRemoved(info.serviceName, type);
                    }
                    //This is ugly - find a cleaner way to do this!
                    int i;
                    for (i = 0; i < mServices.count(); ++i) {
                        if (info.serviceName == mServices.at(i).serviceName) {
                            mServices.removeAt(i);
                            break;
                        }
                    }
                }
            }

            plugin->unload();
            plugin->deleteLater();
        }
        ++it;
    }
    mPlugins.clear();
    mPluginMutex.unlock();
    return true;
}
