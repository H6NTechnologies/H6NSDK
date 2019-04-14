/**
 * H6N Software Development Kit
 * Copyright (c) 2019 H6N Technologies, LLC. All rights reserved.
 *
 * This file is subject to the terms and conditions as defined in
 * your H6N Technologies license agreement. THIS IS NOT OPEN-SOURCE
 * OR FREE SOFTWARE. You MUST have a license to use or redistribute
 * these files in source or binary form in any way.
 */

#ifndef _H6NSDK_LIBH6N_H
#define _H6NSDK_LIBH6N_H

#include <libh6n/common.h>
#include <libh6n/interfaces.h>
#include <libh6n/capsule.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * Retrieves a pointer to an interface by the specified name-version pair. The libh6n API tries to remain backwards-
	 * and forwards-compatible, so interfaces are versioned. Libh6n should automatically use the latest version available
	 * by using preprocessor definitions to define structures of a specific version. The correct interface name and version
	 * number can be found in the particular interface's documentation or in the adjacent preprocessor definitions.
	 *
	 * Many interfaces require that you call a free or release function if you're done using it. Check the documentation
	 * for the particular interface for more details.
	 *
	 * @param name the interface name to acquire
	 * @param version the version of the specified interface to acquire
	 * @return a pointer to the requested interface, or 0 (null pointer) if:
	 *          a) the specified interface-version pair does not exist or is unimplemented,
	 *          b) the H6N agent shared library is not loaded, or
	 *          c) if any other error occurs.
	 */
	void* Agent_createInterface(const char* name, int version);

	void* Capsule_createInterface(const char* name, int version);

#ifdef __cplusplus
}
#endif

#endif //_H6NSDK_LIBH6N_H
