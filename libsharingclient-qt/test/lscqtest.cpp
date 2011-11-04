/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "lscqtest.h"

#include <QDebug>

#include <sharingclient.h>
#include <sharingcommon.h>
#include <sharingclientservice.h>
#include <sharingclientservicemodel.h>

SCTest::SCTest(QObject *parent):
	QObject(parent)
{
	SharingClient *client = new SharingClient();

	qDebug("Client services:");
	qDebug() << client->getServiceTypes(ShareTypeImage);

}

