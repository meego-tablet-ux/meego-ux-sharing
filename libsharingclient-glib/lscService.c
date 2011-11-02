/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "lscService_p.h"
#include "lscService.h"


static const GString * lscEmptyString()
{
    static GString * str = NULL;
    if (!str) {
        str = g_string_new("");
    }
    return str;
}


const GString * lscGetServiceName (const LscService * service)
{
    return service ? service->mServiceName : lscEmptyString();
}

const GString * lscGetDisplayName (const LscService * service)
{
    return service ? service->mDisplayName : lscEmptyString();
}

const GString * lscGetIconPath (const LscService * service)
{
    return service ? service->mIconPath : lscEmptyString();
}

const GString * lscGetServiceDesc (const LscService * service)
{
    return service ? service->mServiceDesc : lscEmptyString();
}

const GString * lscGetServiceType (const LscService * service)
{
    return service ? service->mServiceType : lscEmptyString();
}

LscCredentialsState lscGetCredsState (const LscService * service)
{
    return service ? service->mCredsState : LSC_CREDSSTATE_UNKNOWN;
}

gboolean lscGetServiceAvailable (const LscService * service)
{
    return service ? service->mServiceAvail : FALSE;
}

const GString * lscGetServiceStateText (const LscService * service)
{
    return service ? service->mServiceStateText : lscEmptyString();
}

void lscRegisterServiceChangedCallback (LscService * service,
                                        LscServiceChangedCB cb)
{
    if (service) {
        service->mChangedCallback = cb;
    }
}

gboolean lscCanShareType (const LscService * service,
                          const GString * shareType)
{
    if (!service) return FALSE;

    // TODO: dbus call
    return FALSE;
}

const GString * lscGetSettingsURI (const LscService * service,
                                   const GString * platform,
                                   const GString * product)
{
    if (!service) return lscEmptyString();

    // TODO: dbus call
    return lscEmptyString();
}

const GString * lscGetUIName (const LscService * service,
                              const GString * widgeType,
                              const GString * platform,
                              const GString * product,
                              const GString * shareType,
                              guint shareCount)
{
    if (!service) return lscEmptyString();

    // TODO: dbus call
    return lscEmptyString();
}
