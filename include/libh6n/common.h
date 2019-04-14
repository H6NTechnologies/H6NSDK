/**
 * H6N Software Development Kit
 * Copyright (c) 2019 H6N Technologies, LLC. All rights reserved.
 *
 * This file is subject to the terms and conditions as defined in
 * your H6N Technologies license agreement. THIS IS NOT OPEN-SOURCE
 * OR FREE SOFTWARE. You MUST have a license to use or redistribute
 * these files in source or binary form in any way.
 */

#ifndef _H6NSDK_COMMON_H
#define _H6NSDK_COMMON_H

/*
 * Public Macros
 */

#include <stdint.h>

#define CS_IGNORE
#define CS_VALUE_TYPE 
#define CS_READONLY 


#define H6NSDK_INTERFACE(NAME, VER) _H6N__##NAME##_##VER##_
#define H6NSDK_IFACE_STRUCT(NAME, VER) __H6N__##NAME##_##VER##_

#define H6NSDK_VIRTUAL(NAME, RET) RET(*NAME)

/*
 * Internal Macros
 */



#define _H6NSDK_IFACE_BEGIN(N, V) \
	struct H6NSDK_IFACE_STRUCT(N, V); \
	typedef CS_VALUE_TYPE struct H6NSDK_IFACE_STRUCT(N, V)

#define _H6NSDK_IFACE_END(N, V) H6NSDK_INTERFACE(N, V)


/*
 * Common structures
 */

#ifdef CppSharp
typedef CS_VALUE_TYPE struct _H6N_Int128 {
	uint64_t lo;
	uint64_t hi;
} H6N_Int128;
#else
typedef union _H6N_Int128 {
    struct {
        uint64_t lo;
        uint64_t hi;
    } of64;
    uint8_t bytes[8];
#ifdef __SIZEOF_INT128__
    unsigned __int128 value;
#endif
} H6N_Int128;
#endif


typedef H6N_Int128 H6N_PlayerID;
typedef H6N_Int128 H6N_IntegrationID;

#endif // _H6NSDK_COMMON_H
