/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "lmsctest.h"

#include <QDebug>

#include <meegouxsharingclient.h>
#include <meegouxsharingcommon.h>
#include <meegouxsharingclientservice.h>
#include <meegouxsharingclientservicemodel.h>

    MSCTest::MSCTest(QObject *parent):
    QObject(parent)
    {
    	MeeGoUXSharingClient *client = new MeeGoUXSharingClient();

	qDebug("Client services:");
	qDebug() << client->getServiceTypes(ShareTypeImage);

    }

