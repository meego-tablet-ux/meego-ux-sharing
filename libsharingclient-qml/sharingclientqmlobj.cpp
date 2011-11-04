/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "sharingclientqmlobj.h"
#include <sharingclientservice.h>
#include <sharingclient.h>

SharingClientQmlObj::SharingClientQmlObj(QObject *parent) :
    QObject(parent),
    mClient(new SharingClient()),
    mService(0)
{
    connect(mClient,
            SIGNAL(ShareProgress(QString,int,int,QString)),
            this,
            SIGNAL(shareProgress(QString,int,int,QString)));
}

SharingClientQmlObj::ShareType SharingClientQmlObj::getShareType()
{
    return mShareType;
}

void SharingClientQmlObj::setShareType(SharingClientQmlObj::ShareType type)
{
    mShareType = type;
    mCustomShareType = "";
    emit this->shareTypeChanged();
    emit this->ShareTypeNameChanged();
    emit this->ServiceTypesChanged();
}

QString SharingClientQmlObj::getShareTypeName()
{
    if (mCustomShareType == "")
        return SharingClient::TranslateShareType((SharingType)mShareType);
    else
        return mCustomShareType;
}

QString SharingClientQmlObj::getCustomShareType()
{
    return mCustomShareType;
}

void SharingClientQmlObj::setCustomShareType(QString customType)
{
    mCustomShareType = customType;
}

QStringList SharingClientQmlObj::getServiceTypes()
{
    if (mCustomShareType == "")
        return mClient->getServiceTypes((SharingType)mShareType);
    else
        return mClient->getServiceTypesCustom(mCustomShareType);
}

QString SharingClientQmlObj::getServiceType()
{
    return mServiceType;
}

void SharingClientQmlObj::setServiceType(QString serviceType)
{
    mServiceType = serviceType;
    emit this->ServiceTypeChanged();
}

SharingClientServiceModel * SharingClientQmlObj::getServiceModel()
{
    if (mCustomShareType == "")
        return mClient->getServiceModel(mServiceType, (SharingType)mShareType);
    else
        return mClient->getServiceModelCustom(mServiceType, mCustomShareType);
}

QString SharingClientQmlObj::getServiceName()
{
    if (mService)
        return mService->getServiceName();
    else
        return QString();
}

void SharingClientQmlObj::setServiceName(QString serviceName)
{
    if (mService) {
        if (mService->getServiceName() != serviceName) {
            delete mService;
            mService = new SharingClientService(serviceName);
        }
    } else {
        mService = new SharingClientService(serviceName);
    }
    emit this->ServiceNameChanged();
}

QString SharingClientQmlObj::getCustomUIName(QString platform, QString product)
{
    if (!mService)
        return QString();
    QString shareType = (mCustomShareType == "" ?
                         SharingClient::TranslateShareType((SharingType)mShareType) :
                         mCustomShareType);
    //We can assume that the extension is QML, as this is a QML library...
    return mService->getUIName("QML", platform, product, shareType, mItems.count()).append(".qml");
}

QStringList SharingClientQmlObj::getFilesToShare()
{
    return mItems.keys();
}

uint SharingClientQmlObj::getCredsState()
{
    if (!mService) {
        return CredsStateUnknown;
    } else {
        return mService->getCredsState();
    }
}

QString SharingClientQmlObj::getSettingsURI(QString platform, QString product)
{
    if (!mService) {
        return "";
    } else {
        return mService->getSettingsURI(platform, product);
    }
}


void SharingClientQmlObj::addFile(QString file)
{
    ShareItemStruct sis;
    sis.shareURI = file;
    mItems.insert(file, sis);
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}

void SharingClientQmlObj::addFiles(QStringList files)
{
    foreach (QString file, files) {
        addFile(file);
    }
}

void SharingClientQmlObj::removeFile(QString file)
{
    mItems.remove(file);
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}

void SharingClientQmlObj::removeFiles(QStringList files)
{
    foreach (QString file, files) {
        removeFile(file);
    }
}

QVariant SharingClientQmlObj::getHashVariantForFile(QString file)
{
    if (!mItems.contains(file))
        return QVariant();

    //Have to convert to a <QString, QVariant>-based hash for QVariant to be happy... :(
    QHash<QString, QVariant> theHash;
    ShareParams::const_iterator i = mItems[file].params.constBegin();
    while (i != mItems[file].params.constEnd()) {
        theHash.insert(i.key(), i.value());
        ++i;
    }

    return QVariant(theHash);
}

void SharingClientQmlObj::setHashVariantForFile(QString file, QVariant hash)
{
    if (!mItems.contains(file))
        return;

    //Convert back from a <QString, QVariant>-based hash to our <QString, QString>-based hash
    QHash<QString, QVariant> theHash = hash.toHash();
    QHash<QString, QVariant>::const_iterator i = theHash.constBegin();
    while (i != theHash.constEnd()) {
        addHashEntryToFile(file, i.key(), i.value().toString());
        ++i;
    }
}

QStringList SharingClientQmlObj::getHashKeysForFile(QString file)
{
    if (!mItems.contains(file))
        return QStringList();

    return mItems[file].params.keys();
}

void SharingClientQmlObj::addHashEntryToFile(QString file, QString paramName, QString paramVal)
{
    //qDebug() << QString("addHashEntryToFile called, fileID %1, count %2").arg(fileID, mItems.count());
    //qDebug() << QString("paramName: %1, paramVal: %2").arg(paramName, paramVal);
    if (!mItems.contains(file))
        return;

    mItems[file].params.insert(paramName, paramVal);
//    mItems.replace(fileID, sis);
    //qDebug() << QString("sis.params: ") << sis.params;
//    qDebug() << QString

}

void SharingClientQmlObj::modifyHashEntryForFile(QString file, QString paramName, QString newParamVal)
{
    if (!mItems.contains(file))
        return;

    if (!mItems[file].params.contains(paramName))
        return;

    mItems[file].params[paramName] = newParamVal;
}

QString SharingClientQmlObj::getHashEntryForFile(QString file, QString paramName, QString defaultVal)
{
    if (!mItems.contains(file))
        return defaultVal;

    return mItems[file].params.value(paramName, defaultVal);
}

int SharingClientQmlObj::shareSimple()
{
    if (!mService) {
        return -1;
    }

    ArrayOfShareItemStruct arrItems;

    foreach (ShareItemStruct item, mItems.values()) {
        arrItems.append(item);
    }

    return mClient->shareSimple(mService->getServiceName(), getShareTypeName(), arrItems);
    //return mService->shareSimple(arrItems);
}

QString SharingClientQmlObj::getLastShareError()
{
    if (!mService)
        return QString("Null mService!");
    else
        return mClient->getLastShareError();
}

void SharingClientQmlObj::clearFiles()
{
    mItems.clear();
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}
