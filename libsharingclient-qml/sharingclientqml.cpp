/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "sharingclientqml.h"

#include <sharingclient.h>
#include <sharingclientservice.h>
#include <sharingclientservicemodel.h>
#include "sharingclientqmlobj.h"

void SharingClientQml::registerTypes(const char *uri)
{
//    qmlRegisterType<SharingClient>(uri, 0, 0, "SharingClient");
//    qmlRegisterType<SharingClientService>(uri, 0, 0, "SharingClientService");
    qmlRegisterType<SharingClientServiceModel>(uri, 0, 0, "SharingClientServiceModel");
    qmlRegisterType<SharingClientQmlObj>(uri, 0, 0, "SharingClientQmlObj");
}

Q_EXPORT_PLUGIN(SharingClientQml);
