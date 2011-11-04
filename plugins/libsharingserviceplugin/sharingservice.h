/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGSERVICE_H
#define SHARINGSERVICE_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <sharingcommon.h>
#include <sharing-service_adaptor.h>

class SharingService : public QObject {
    Q_OBJECT

public:
    explicit SharingService(const SharingServiceInfo &serviceInfo, QObject *parent = 0);
    virtual ~SharingService() = 0;

    virtual QString GetDBusObjPath() const;

public slots:
    //For ServiceAdaptor auto-invoking
    virtual bool CanShareType(const QString &sharetype) = 0;
    virtual uint GetCredsState() = 0;
    virtual QString GetDisplayName() = 0;
    virtual QString GetIconPath() = 0;
    virtual bool GetServiceAvailable() = 0;
    virtual QString GetServiceDesc() = 0;
    virtual QString GetServiceName() = 0;
    virtual QString GetServiceStateText() = 0;
    virtual QString GetServiceType() = 0;
    virtual QString GetSettingsURI(const QString &platform, const QString &product) = 0;
    virtual QString GetUIName(const QString &widgettype, const QString &platform, const QString &product, const QString &sharetype, uint sharecount) = 0;

signals:
    //For ServiceAdaptor auto-connecting
    void ServiceChanged(const QString &service);

protected:
    ServiceAdaptor *mServiceAdaptor;
    SharingServiceInfo mServiceInfo;

};

Q_DECLARE_METATYPE(SharingService *)

#endif //SHARINGSERVICE_H
