/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGCLIENTSERVICE_H
#define MEEGOUXSHARINGCLIENTSERVICE_H

#include <QObject>
#include <QString>
#include <QtDBus>
#include <QMetaType>

#include <meegouxsharingdbustypes.h>

class ComMeegoUxSharingServiceInterface;

class MeeGoUXSharingClientService : public QObject
{
    Q_OBJECT
public:
    explicit MeeGoUXSharingClientService(QString serviceName = QString(),
                                       const QDBusConnection &conn = QDBusConnection::sessionBus(),
                                       QObject *parent = 0);
    Q_PROPERTY(QString serviceName READ getServiceName WRITE setServiceName);

    Q_INVOKABLE void setServiceName(QString serviceName);
    Q_INVOKABLE QString getServiceName();
    Q_INVOKABLE QString getDisplayName();
    Q_INVOKABLE QString getIconPath();
    Q_INVOKABLE QString getServiceDesc();
    Q_INVOKABLE QString getServiceType();

    Q_INVOKABLE bool canShareType(const QString &sharetype);
    Q_INVOKABLE uint getCredsState();
    Q_INVOKABLE bool getServiceAvailable();

    Q_INVOKABLE QString getServiceStateText();
    Q_INVOKABLE QString getSettingsURI(const QString &platform, const QString &product);
    Q_INVOKABLE QString getUIName(const QString &widgettype, const QString &platform, const QString &product, const QString &sharetype, uint sharecount);

signals:
    void ServiceChanged(MeeGoUXSharingClientService *);

public slots:

protected slots:
    void onServiceChanged(const QString &service);

protected:
    void loadServiceData();

private:
    ComMeegoUxSharingServiceInterface *mServiceIntf;
    QDBusConnection mConn;

    QString mServiceName;
    QString mDisplayName;
    QString mIconPath;
    QString mServiceDesc;
    QString mServiceType;
    uint    mCredsState;
    bool    mServiceAvail;
    QString mServiceStateText;

};

Q_DECLARE_METATYPE(MeeGoUXSharingClientService *)

#endif // MEEGOUXSHARINGCLIENTSERVICE_H
