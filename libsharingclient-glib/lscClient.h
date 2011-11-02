/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LIBSHARINGCLIENT_GLIB_CLIENT_H
#define LIBSHARINGCLIENT_GLIB_CLIENT_H

#include "lscItem.h"
#include "lscService.h"
#include <glib.h>

//! Callback function type for receiving share progress
typedef void (*LscProgressCB) (const LscService * service,
                               gint opid,
                               gint progress,
                               GString message);


//! Array of LscService pointers.  Return array is owned by the caller.
const GArray * lscGetServices (const GArray * items);

//! @returns operation ID (opid)
gint lscShare (LscService * service,
               const GArray * items,
               GString * errorMessage);

gboolean lscCancelShare (gint opid);

void lscRegisterProgressCallback(gint opid,
                                 LscProgressCB cb);

#endif //LIBSHARINGCLIENT_GLIB_CLIENT_H
