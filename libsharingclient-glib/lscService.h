/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LIBSHARINGCLIENT_GLIB_SERVICE_H
#define LIBSHARINGCLIENT_GLIB_SERVICE_H

#include "lscCommon.h"
#include <glib.h>

typedef struct LscService_ LscService;

//! Callback function type for receiving share progress
typedef void (*LscServiceChangedCB) (const LscService * service);

const GString * lscGetServiceName (const LscService * service);

const GString * lscGetDisplayName (const LscService * service);

const GString * lscGetIconPath (const LscService * service);

const GString * lscGetServiceDesc (const LscService * service);

const GString * lscGetServiceType (const LscService * service);

gboolean lscCanShareType (const LscService * service,
                          const GString * shareType);

LscCredentialsState lscGetCredsState (const LscService * service);

gboolean lscGetServiceAvailable (const LscService * service);

const GString * lscGetServiceStateText (const LscService * service);

const GString * lscGetSettingsURI (const LscService * service,
                                   const GString * platform,
                                   const GString * product);

const GString * lscGetUIName (const LscService * service,
                              const GString * widgeType,
                              const GString * platform,
                              const GString * product,
                              const GString * shareType,
                              guint shareCount);

void lscRegisterServiceChangedCallback (LscService * service,
                                        LscServiceChangedCB cb);


#endif // LIBSHARINGCLIENT_GLIB_SERVICE_H
