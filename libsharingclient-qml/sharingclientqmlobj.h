/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef SHARINGCLIENTQMLOBJ_H
#define SHARINGCLIENTQMLOBJ_H

#include <QObject>
#include <QHash>
#include <sharingdbustypes.h>
#include <sharingcommon.h>

//This class is used to access the sharing framework from QML


class SharingClient;
class SharingClientService;
class SharingClientServiceModel;

class SharingClientQmlObj : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ShareType shareType READ getShareType WRITE setShareType NOTIFY shareTypeChanged)
    Q_PROPERTY(QString shareTypeName READ getShareTypeName NOTIFY ShareTypeNameChanged)
    Q_PROPERTY(QString customShareType READ getCustomShareType WRITE setCustomShareType)
    Q_PROPERTY(QStringList serviceTypes READ getServiceTypes NOTIFY ServiceTypesChanged)
    Q_PROPERTY(QString serviceType READ getServiceType WRITE setServiceType NOTIFY ServiceTypeChanged)
    Q_PROPERTY(SharingClientServiceModel * serviceModel READ getServiceModel)
    Q_PROPERTY(QString serviceName READ getServiceName WRITE setServiceName NOTIFY ServiceNameChanged)
    Q_PROPERTY(QStringList filesToShare READ getFilesToShare NOTIFY FilesToShareChanged)
    Q_PROPERTY(uint fileCount READ getFileCount NOTIFY FileCountChanged)

    //TODO - figure out how to expose this type to QML from the sharingcommon.h file
    Q_ENUMS(ShareType)
    Q_ENUMS(CredsState)


public:
    explicit SharingClientQmlObj(QObject *parent = 0);

    enum ShareType {
        ShareTypeImage = 0,
        ShareTypeVideo = 1,
        ShareTypeAudio = 2,
        ShareTypeText  = 3,
        ShareTypeVCard = 4,
    };

    enum CredsState {
        CredsStateValid = 0,
        CredsStateInvalid = 1,
        CredsStateUnknown = 2,
    };

    //First, you set the share type
    Q_INVOKABLE SharingClientQmlObj::ShareType getShareType();
    Q_INVOKABLE void setShareType(SharingClientQmlObj::ShareType type);

    Q_INVOKABLE QString getShareTypeName();

    //Or a custom share type
    Q_INVOKABLE QString getCustomShareType();
    Q_INVOKABLE void setCustomShareType(QString customType);

    //Then you get the string list of the available service types for that share type
    Q_INVOKABLE QStringList getServiceTypes();

    //Then you pick one of the service types and set it
    Q_INVOKABLE QString getServiceType();
    Q_INVOKABLE void setServiceType(QString serviceType);

    //Then you can get the service model that gives you a list of services for that share type and service type
    Q_INVOKABLE SharingClientServiceModel * getServiceModel();

    //Then you set the service name that you want to use - once you do that, you can share at will!
    Q_INVOKABLE QString getServiceName();
    Q_INVOKABLE void setServiceName(QString serviceName);

    //This is used by the Sharing UI in order to get the custom QML UI filename to dynamically load
    Q_INVOKABLE QString getCustomUIName(QString platform, QString product);

    //This is how you get a list of which files are currently selected to share
    Q_INVOKABLE QStringList getFilesToShare();

    //Get the credentials state of the service
    Q_INVOKABLE uint getCredsState();

    Q_INVOKABLE QString getSettingsURI(QString platform, QString product);

    //Now you can manipulate the share items - note that the files will actually be added
    //by the app that is calling the sharing UI widget, so those should be pre-populated.

    //Used to add 1 file at a time
    Q_INVOKABLE void addFile(QString file);
    //For adding multiple files
    Q_INVOKABLE void addFiles(QStringList files);
    Q_INVOKABLE void removeFile(QString file);
    Q_INVOKABLE void removeFiles(QStringList files);
    Q_INVOKABLE QVariant getHashVariantForFile(QString file);
    Q_INVOKABLE void setHashVariantForFile(QString file, QVariant hash);
    Q_INVOKABLE QStringList getHashKeysForFile(QString file);
    Q_INVOKABLE void addHashEntryToFile(QString file, QString paramName, QString paramVal);
    Q_INVOKABLE void modifyHashEntryForFile(QString file, QString paramName, QString newParamVal);
    Q_INVOKABLE QString getHashEntryForFile(QString file, QString paramName, QString defaultVal = "");
    Q_INVOKABLE int shareSimple();
    Q_INVOKABLE QString getLastShareError();
    Q_INVOKABLE void clearFiles();
    Q_INVOKABLE uint getFileCount() { return mItems.count(); }

signals:
    void shareTypeChanged();
    void ShareTypeNameChanged();
    void ServiceTypeChanged();
    void ServiceNameChanged();
    void FilesToShareChanged();
    void FileCountChanged();
    void ServiceTypesChanged();
    void shareProgress(QString serviceName, int opid, int progress, const QString &message);

private:
    SharingClient *mClient;
    SharingClientService *mService;
    QHash<QString, ShareItemStruct> mItems;
    SharingClientQmlObj::ShareType mShareType;
    QString mCustomShareType;
    QString mServiceType;

};

#endif // SHARINGCLIENTQMLOBJ_H
