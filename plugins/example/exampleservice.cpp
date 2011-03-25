/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "exampleservice.h"

#include <QFile>

ExampleService::ExampleService(MeeGoUXSharingServiceInfo serviceInfo, QObject *parent):
    MeeGoUXSharingService(serviceInfo, parent),
    mShareID(0)
{
    qDebug() << QString("Starting up ExampleService with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
}

ExampleService::~ExampleService()
{
    qDebug() << QString("Shutting down ExampleService with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
}

bool ExampleService::CanShareType(const QString &sharetype)
{
    qDebug() << QString("ExampleService::CanShareType with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return (mServiceInfo.serviceTypes.contains(sharetype));
}

bool ExampleService::CancelShare(int opid)
{
    qDebug() << QString("ExampleService::CancelShare with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    qDebug() << QString("Canceling opid %1!").arg(opid);
    return true;
}

uint ExampleService::GetCredsState()
{
    qDebug() << QString("ExampleService::GetCredsState with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return CredsStateValid;
}

QString ExampleService::GetDisplayName()
{
    qDebug() << QString("ExampleService::GetDisplayName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return QString("Translated DisplayName %1!").arg(mServiceInfo.serviceName);
}

QString ExampleService::GetIconPath()
{
    qDebug() << QString("ExampleService::GetIconPath with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return QString("/usr/share/my_icon_path");
}

bool ExampleService::GetServiceAvailable()
{
    qDebug() << QString("ExampleService::GetServiceAvailable with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return true;
}

QString ExampleService::GetServiceDesc()
{
    qDebug() << QString("ExampleService::GetServiceDesc with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return QString("The Example plugin is used for reference on how to plug services in to the MeeGo Sharing API/framework.");
}

QString ExampleService::GetServiceName()
{
    qDebug() << QString("ExampleService::GetServiceName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return mServiceInfo.serviceName;
}

QString ExampleService::GetServiceStateText()
{
    qDebug() << QString("ExampleService::GetServiceStateText with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return QString("Dummy state text for example %1!").arg(mServiceInfo.serviceName);
}

QString ExampleService::GetServiceType()
{
    qDebug() << QString("ExampleService::GetServiceType with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    if (mServiceInfo.serviceName == "foo")
        return "Web";
    else
        return "Email";
}

QString ExampleService::GetSettingsURI(const QString &platform,
                                       const QString &product)
{
    qDebug() << QString("ExampleService::GetSettingsURI with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    return QString("exec echo \"Launching settings for service %1, on platform %2, product %3\"").arg(mServiceInfo.serviceName, platform, product);
}

QString ExampleService::GetUIName(const QString &widgettype,
                                  const QString &platform,
                                  const QString &product,
                                  const QString &sharetype,
                                  uint sharecount)
{
    qDebug() << QString("ExampleService::GetUIName with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);

    if (sharetype == MEEGO_SHARE_TYPE_AUDIO)
        return QString();

    QString type;
    QString mult;
    if (widgettype == "QML") {
        if (sharetype == MEEGO_SHARE_TYPE_IMAGE) {
            type = "image";
        } else if (sharetype == MEEGO_SHARE_TYPE_VIDEO) {
            type = "video";
        } else {
            type = QString(sharetype).replace(QString("/"), QString("_"));   //Custom type support
        }

        mult = (sharecount > 1 ? "multi" : "single");
        QString filename = QString("%1/%2/%3_%4_%5_%6_%7").arg(QML_TARGET_BASE_PATH, "example", mServiceInfo.serviceName, platform, product, type, mult);
        //If we don't have a file for this prodct/platform
        if (!QFile::exists(filename + ".qml")) {
            //Try just this product
            filename = QString("%1/%2/%3_%4_%5_%6").arg(QML_TARGET_BASE_PATH, "example", mServiceInfo.serviceName, platform, type, mult);
            if (!QFile::exists(filename + ".qml")) {
                //Otherwise, just return the standard one
                filename = QString("%1/%2/%3_%4_%5").arg(QML_TARGET_BASE_PATH, "example", mServiceInfo.serviceName, type, mult);
            }
        }
        return filename;

    } else {
        //Handle other UI types here...
        return QString();
    }
}

int ExampleService::Share(const QString &sharetype, ArrayOfShareItemStruct items, QString &errmessage)
{
    if (!CanShareType(sharetype)) {
        errmessage = QString("Invalid share type %1!").arg(sharetype);
	return -1;
    }
    if (items.count() < 1) {
        errmessage = "No items to share!";
        return -1;
    }
    qDebug() << QString("ExampleService::Share with mServiceInfo.serviceName of %1!").arg(mServiceInfo.serviceName);
    qDebug() << QString("Assigning shareID of %1").arg(++mShareID);
    qDebug() << QString("Number of items: %1").arg(items.count());
    foreach(ShareItemStruct sis, items) {
        qDebug() << QString("Received file %1!").arg(sis.shareURI);
        qDebug() << sis.params;
    }

    return mShareID;
}


