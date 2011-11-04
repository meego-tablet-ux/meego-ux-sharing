/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGCLIENT_H
#define SHARINGCLIENT_H

#include <QObject>
#include <QtDBus>
#include <QStringList>
#include <sharingcommon.h>
#include <sharingdbustypes.h>

class SharingClientServiceModel;
class SharingClientCore;

class SharingClient: public QObject {
    Q_OBJECT

public:
    SharingClient(const QDBusConnection &connection = QDBusConnection::sessionBus(),
                       QObject *parent = 0);

    Q_INVOKABLE static QString TranslateShareType(SharingType type);

    Q_INVOKABLE QStringList getServiceTypes(SharingType type);
    Q_INVOKABLE QStringList getServiceTypesCustom(QString customType);

    Q_INVOKABLE SharingClientServiceModel * getServiceModel(QString serviceType, SharingType type);
    Q_INVOKABLE SharingClientServiceModel * getServiceModelCustom(QString serviceType, QString customType);
    //Not Q_INVOKABLE, as I haven't yet figured out how to get QML to understand an ArrayOfShareItemStruct types. See the libsharingclient-qml library for further details.
    int share(QString serviceName, QString sharingType, ArrayOfShareItemStruct items, QString &errmessage);
    Q_INVOKABLE int shareSimple(QString serviceName, QString sharingType, ArrayOfShareItemStruct items);
    Q_INVOKABLE QString getLastShareError();
    Q_INVOKABLE bool cancelShare(int opid);



protected slots:
//    void onServiceAdded(QString, QString);
//    void onServiceRemoved(QString, QString);
    void onShareProgress(const QString &service, int opid, int progress, const QString &message);

signals:
    void ShareProgress(QString serviceName, int opid, int progress, const QString &message);

private:
    SharingClientCore *mCore;
    QStringList mServices;
    QMap<int, int> mShareItemStatuses;
    QString mLastShareError;

};


#endif // SHARINGCLIENT_H
