/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LIBSHARINGCLIENT_GLIB_COMMON_H
#define LIBSHARINGCLIENT_GLIB_COMMON_H

#include <glib.h>

typedef enum  {
    LSC_CREDSSTATE_VALID   = 0,
    LSC_CREDSSTATE_INVALID = 1,
    LSC_CREDSSTATE_UNKNOWN = 2
} LscCredentialsState;

#endif //LIBSHARINGCLIENT_GLIB_COMMON_H
