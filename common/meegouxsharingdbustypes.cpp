/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 	
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "meegouxsharingdbustypes.h"


QDBusArgument &operator<<(QDBusArgument &arg, const ShareItemStruct &item)
{
    arg.beginStructure();
    arg << item.shareURI << item.params;
    arg.endStructure();
    return arg;
}

const QDBusArgument &operator>>(const QDBusArgument &arg, ShareItemStruct &item)
{
    arg.beginStructure();
    arg >> item.shareURI >> item.params;
    arg.endStructure();
    return arg;
}
