/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGOUXSHARINGCLIENTSERVICEMODEL_H
#define MEEGOUXSHARINGCLIENTSERVICEMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QtDBus>

class MeeGoUXSharingClientCore;
class MeeGoUXSharingClientService;

class MeeGoUXSharingClientServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MeeGoUXSharingClientServiceModel(QString serviceType = QString(),
                                            QString shareType = QString(),
                                      MeeGoUXSharingClientCore *core = 0,
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
    //void ShareProgress(MeeGoUXSharingClientService *, int opid, int progress, QString message);

public slots:

protected slots:
    void onServiceChanged(MeeGoUXSharingClientService *);
    void onServiceAdded(QString name, QString shareType);
    void onServiceRemoved(QString name, QString shareType);

private:
    void initialize();
    void filter();

    QStringList mServiceList;
    QList<MeeGoUXSharingClientService *> mServices;
    MeeGoUXSharingClientCore *mCore;
    QString mServiceType;
    QString mShareType;
    bool mInitialized;
};

#endif // MEEGOUXSHARINGCLIENTSERVICEMODEL_H
