/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingclientqmlobj.h"
#include <meegouxsharingclientservice.h>
#include <meegouxsharingclient.h>

MeeGoUXSharingClientQmlObj::MeeGoUXSharingClientQmlObj(QObject *parent) :
    QObject(parent),
    mClient(new MeeGoUXSharingClient()),
    mService(0)
{
    connect(mClient,
            SIGNAL(ShareProgress(QString,int,int,QString)),
            this,
            SIGNAL(shareProgress(QString,int,int,QString)));
}

MeeGoUXSharingClientQmlObj::ShareType MeeGoUXSharingClientQmlObj::getShareType()
{
    return mShareType;
}

void MeeGoUXSharingClientQmlObj::setShareType(MeeGoUXSharingClientQmlObj::ShareType type)
{
    mShareType = type;
    mCustomShareType = "";
    emit this->shareTypeChanged();
    emit this->ShareTypeNameChanged();
    emit this->ServiceTypesChanged();
}

QString MeeGoUXSharingClientQmlObj::getShareTypeName()
{
    if (mCustomShareType == "")
        return MeeGoUXSharingClient::TranslateShareType((MeeGoUXSharingType)mShareType);
    else
        return mCustomShareType;
}

QString MeeGoUXSharingClientQmlObj::getCustomShareType()
{
    return mCustomShareType;
}

void MeeGoUXSharingClientQmlObj::setCustomShareType(QString customType)
{
    mCustomShareType = customType;
}

QStringList MeeGoUXSharingClientQmlObj::getServiceTypes()
{
    if (mCustomShareType == "")
        return mClient->getServiceTypes((MeeGoUXSharingType)mShareType);
    else
        return mClient->getServiceTypesCustom(mCustomShareType);
}

QString MeeGoUXSharingClientQmlObj::getServiceType()
{
    return mServiceType;
}

void MeeGoUXSharingClientQmlObj::setServiceType(QString serviceType)
{
    mServiceType = serviceType;
    emit this->ServiceTypeChanged();
}

MeeGoUXSharingClientServiceModel * MeeGoUXSharingClientQmlObj::getServiceModel()
{
    if (mCustomShareType == "")
        return mClient->getServiceModel(mServiceType, (MeeGoUXSharingType)mShareType);
    else
        return mClient->getServiceModelCustom(mServiceType, mCustomShareType);
}

QString MeeGoUXSharingClientQmlObj::getServiceName()
{
    if (mService)
        return mService->getServiceName();
    else
        return QString();
}

void MeeGoUXSharingClientQmlObj::setServiceName(QString serviceName)
{
    if (mService) {
        if (mService->getServiceName() != serviceName) {
            delete mService;
            mService = new MeeGoUXSharingClientService(serviceName);
        }
    } else {
        mService = new MeeGoUXSharingClientService(serviceName);
    }
    emit this->ServiceNameChanged();
}

QString MeeGoUXSharingClientQmlObj::getCustomUIName(QString platform, QString product)
{
    if (!mService)
        return QString();
    QString shareType = (mCustomShareType == "" ?
                         MeeGoUXSharingClient::TranslateShareType((MeeGoUXSharingType)mShareType) :
                         mCustomShareType);
    //We can assume that the extension is QML, as this is a QML library...
    return mService->getUIName("QML", platform, product, shareType, mItems.count()).append(".qml");
}

QStringList MeeGoUXSharingClientQmlObj::getFilesToShare()
{
    return mItems.keys();
}

uint MeeGoUXSharingClientQmlObj::getCredsState()
{
    if (!mService) {
        return CredsStateUnknown;
    } else {
        return mService->getCredsState();
    }
}

QString MeeGoUXSharingClientQmlObj::getSettingsURI(QString platform, QString product)
{
    if (!mService) {
        return "";
    } else {
        return mService->getSettingsURI(platform, product);
    }
}


void MeeGoUXSharingClientQmlObj::addFile(QString file)
{
    ShareItemStruct sis;
    sis.shareURI = file;
    mItems.insert(file, sis);
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}

void MeeGoUXSharingClientQmlObj::addFiles(QStringList files)
{
    foreach (QString file, files) {
        addFile(file);
    }
}

void MeeGoUXSharingClientQmlObj::removeFile(QString file)
{
    mItems.remove(file);
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}

void MeeGoUXSharingClientQmlObj::removeFiles(QStringList files)
{
    foreach (QString file, files) {
        removeFile(file);
    }
}

QVariant MeeGoUXSharingClientQmlObj::getHashVariantForFile(QString file)
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

void MeeGoUXSharingClientQmlObj::setHashVariantForFile(QString file, QVariant hash)
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

QStringList MeeGoUXSharingClientQmlObj::getHashKeysForFile(QString file)
{
    if (!mItems.contains(file))
        return QStringList();

    return mItems[file].params.keys();
}

void MeeGoUXSharingClientQmlObj::addHashEntryToFile(QString file, QString paramName, QString paramVal)
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

void MeeGoUXSharingClientQmlObj::modifyHashEntryForFile(QString file, QString paramName, QString newParamVal)
{
    if (!mItems.contains(file))
        return;

    if (!mItems[file].params.contains(paramName))
        return;

    mItems[file].params[paramName] = newParamVal;
}

QString MeeGoUXSharingClientQmlObj::getHashEntryForFile(QString file, QString paramName, QString defaultVal)
{
    if (!mItems.contains(file))
        return defaultVal;

    return mItems[file].params.value(paramName, defaultVal);
}

int MeeGoUXSharingClientQmlObj::shareSimple()
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

QString MeeGoUXSharingClientQmlObj::getLastShareError()
{
    if (!mService)
        return QString("Null mService!");
    else
        return mClient->getLastShareError();
}

void MeeGoUXSharingClientQmlObj::clearFiles()
{
    mItems.clear();
    emit this->FileCountChanged();
    emit this->FilesToShareChanged();
}
