/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LIBSHARINGCLIENT_GLIB_ITEM_H
#define LIBSHARINGCLIENT_GLIB_ITEM_H

#include <glib.h>

struct LscShareItem {
    GString shareURI;
    GString shareType;
    GHashTable * params;  // GString keys, GString values
};

#endif // LIBSHARINGCLIENT_GLIB_ITEM_H
