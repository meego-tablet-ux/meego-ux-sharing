/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef EXAMPLESERVICE_H
#define EXAMPLESERVICE_H

#include <QObject>
#include <QMetaType>
#include <meegouxsharingservice.h>
#include <meegouxsharingcommon.h>

class ExampleService : public MeeGoUXSharingService
{
    Q_OBJECT
public:
    explicit ExampleService(MeeGoUXSharingServiceInfo serviceInfo, QObject *parent = 0);
    ~ExampleService();

public slots:
    //For ServiceAdaptor auto-invoking
    bool CanShareType(const QString &sharetype);
    bool CancelShare(int opid);
    uint GetCredsState();
    QString GetDisplayName();
    QString GetIconPath();
    bool GetServiceAvailable();
    QString GetServiceDesc();
    QString GetServiceName();
    QString GetServiceStateText();
    QString GetServiceType();
    QString GetSettingsURI(const QString &platform, const QString &product);
    QString GetUIName(const QString &widgettype, const QString &platform, const QString &product, const QString &sharetype, uint sharecount);
    int Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage);

signals:
    //For ServiceAdaptor auto-connecting
    void ServiceChanged(const QString &service);
    void ShareProgress(const QString &service, int opid, int progress, const QString &message);

private:
    uint mShareID;

};

Q_DECLARE_METATYPE(ExampleService *)

#endif // EXAMPLESERVICE_H
