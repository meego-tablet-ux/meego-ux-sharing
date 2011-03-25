/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGCLIENTCORE_H
#define MEEGOUXSHARINGCLIENTCORE_H

#include <QObject>
#include <QtDBus>

#include <meegouxsharingdbustypes.h>

class ComMeegoUxSharingInterface;

class MeeGoUXSharingClientCore : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoUXSharingClientCore(const QDBusConnection &connection = QDBusConnection::sessionBus(),
                                    QObject *parent = 0);

    QStringList getServicesForSharingType(QString sharingType);
    QStringList getServiceTypes(QString sharingType);
    bool cancelShare(int opid);
    int share(QString serviceName, QString sharingType, ArrayOfShareItemStruct items, QString &errMessage);

    const QDBusConnection & getConnection() const;

signals:
    void ServiceAdded(QString name, QString sharingType);
    void ServiceRemoved(QString name, QString sharingType);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

public slots:

protected slots:
    //void onServiceAdded(QString, QString);
    //void onServiceRemoved(QString, QString);

private:
    ComMeegoUxSharingInterface *mDaemonIntf;
    QDBusConnection mConnection;
    QStringList mServicesList;

};

#endif // MEEGOUXSHARINGCLIENTCORE_H
