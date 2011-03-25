/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingclientqml.h"

#include <meegouxsharingclient.h>
#include <meegouxsharingclientservice.h>
#include <meegouxsharingclientservicemodel.h>
#include "meegouxsharingclientqmlobj.h"

void MeeGoUXSharingClientQml::registerTypes(const char *uri)
{
//    qmlRegisterType<MeeGoUXSharingClient>(uri, 0, 0, "MeeGoUXSharingClient");
//    qmlRegisterType<MeeGoUXSharingClientService>(uri, 0, 0, "MeeGoUXSharingClientService");
    qmlRegisterType<MeeGoUXSharingClientServiceModel>(uri, 0, 0, "MeeGoUXSharingClientServiceModel");
    qmlRegisterType<MeeGoUXSharingClientQmlObj>(uri, 0, 0, "MeeGoUXSharingClientQmlObj");
}

Q_EXPORT_PLUGIN(MeeGoUXSharingClientQml);
