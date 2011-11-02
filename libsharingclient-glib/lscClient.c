/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "lscClient.h"
#include "lscService_p.h"

const GArray * lscGetServices (const GArray * items)
{
    GArray * array = g_array_new(FALSE, FALSE, sizeof(LscService *));
    if (items) {
    }
    return array;
}

gint lscShare (LscService * service,
               const GArray * items,
               GString * errorMessage)
{
    return -1;
}

gboolean lscCancelShare (gint opid)
{
    return FALSE;
}

void lscRegisterProgressCallback(gint opid,
                                 LscProgressCB cb)
{
}
