/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGCLIENTSERVICEMODEL_H
#define SHARINGCLIENTSERVICEMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QtDBus>

class SharingClientCore;
class SharingClientService;

class SharingClientServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit SharingClientServiceModel(QString serviceType = QString(),
                                            QString shareType = QString(),
                                      SharingClientCore *core = 0,
                                      QObject *parent = 0);

    Q_PROPERTY(QString serviceType READ getServiceType WRITE setServiceType);
    Q_PROPERTY(QString shareType READ getShareType WRITE setShareType);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE QString getServiceNameFromDisplayName(QString displayName);

    QString getServiceType() { return mServiceType; }
    QString getShareType() { return mShareType; }
    Q_INVOKABLE QStringList getServiceTypesList();

    void setServiceType(QString serviceType);
    void setShareType(QString shareType);

signals:
    //void ShareProgress(SharingClientService *, int opid, int progress, QString message);

public slots:

protected slots:
    void onServiceChanged(SharingClientService *);
    void onServiceAdded(QString name, QString shareType);
    void onServiceRemoved(QString name, QString shareType);

private:
    void initialize();
    void filter();

    QStringList mServiceList;
    QList<SharingClientService *> mServices;
    SharingClientCore *mCore;
    QString mServiceType;
    QString mShareType;
    bool mInitialized;
};

#endif // SHARINGCLIENTSERVICEMODEL_H
