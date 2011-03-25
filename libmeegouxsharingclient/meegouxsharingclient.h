/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGCLIENT_H
#define MEEGOUXSHARINGCLIENT_H

#include <QObject>
#include <QtDBus>
#include <QStringList>
#include <meegouxsharingcommon.h>
#include <meegouxsharingdbustypes.h>

class MeeGoUXSharingClientServiceModel;
class MeeGoUXSharingClientCore;

class MeeGoUXSharingClient: public QObject {
    Q_OBJECT

public:
    MeeGoUXSharingClient(const QDBusConnection &connection = QDBusConnection::sessionBus(),
                       QObject *parent = 0);

    Q_INVOKABLE static QString TranslateShareType(MeeGoUXSharingType type);

    Q_INVOKABLE QStringList getServiceTypes(MeeGoUXSharingType type);
    Q_INVOKABLE QStringList getServiceTypesCustom(QString customType);

    Q_INVOKABLE MeeGoUXSharingClientServiceModel * getServiceModel(QString serviceType, MeeGoUXSharingType type);
    Q_INVOKABLE MeeGoUXSharingClientServiceModel * getServiceModelCustom(QString serviceType, QString customType);
    //Not Q_INVOKABLE, as I haven't yet figured out how to get QML to understand an ArrayOfShareItemStruct types. See the libmeegouxsharingclientqml library for further details.
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
    MeeGoUXSharingClientCore *mCore;
    QStringList mServices;
    QMap<int, int> mShareItemStatuses;
    QString mLastShareError;

};


#endif // MEEGOUXSHARINGCLIENT_H
