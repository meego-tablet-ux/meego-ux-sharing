/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QCoreApplication>
#include "sharingdaemon.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    SharingDaemon *sd = new SharingDaemon(&app);
    if (sd->init())
        return app.exec();
    else
        return 1;
}
