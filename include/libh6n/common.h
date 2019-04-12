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

#define H6NSDK_INTERFACE(NAME, VER) _H6N__##NAME##_##VER##_
#define H6NSDK_IFACE_STRUCT(NAME, VER) __H6N__##NAME##_##VER##_

#define H6NSDK_VIRTUAL(NAME, RET) RET(*NAME)

/*
 * Internal Macros
 */

#define CS_IGNORE
#define CS_VALUE_TYPE 
#define CS_READONLY 


#define _H6NSDK_IFACE_BEGIN(N, V) typedef CS_VALUE_TYPE struct H6NSDK_IFACE_STRUCT(N, V)
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

#endif // _H6NSDK_COMMON_H

/**
* H6N Software Development Kit
* Copyright (c) 2019 H6N Technologies, LLC. All rights reserved.
*
* This file is subject to the terms and conditions as defined in
* your H6N Technologies license agreement. THIS IS NOT OPEN-SOURCE
* OR FREE SOFTWARE. You MUST have a license to use or redistribute
* these files in source or binary form in any way.
*/

#ifndef _H6NSDK_INTERFACES_H
#define _H6NSDK_INTERFACES_H

#include <libh6n/common.h>

#ifdef __cplusplus
extern "C" {
#endif


#define H6AC_CLIENT_VERSION 1
#define H6AC_CLIENT_INTERFACE "H6ACClient"

	/**
	* `H6ACClient` is the interface that game clients interact with H6AC, the H6N Technologies Anti-Cheat.
	* Use the `H6AC` macro definition to use the latest version of the interface available for use with this SDK version.
	* We try our hardest to keep this interface backwards compatible through versioning -- that is, you shouldn't ever
	* *have* to upgrade to a newer SDK version to continue using H6AC, you may just miss out on any new features.
	*
	* Interface name defined in H6AC_INTERFACE as "H6ACClient"
	* Current interface version defined in H6AC_VERSION as 1
	*/
	_H6NSDK_IFACE_BEGIN(H6ACClient, 1) {

		/**
		* Frees the structure.
		* @param thiz pointer to the H6AC instance
		*/
		H6NSDK_VIRTUAL(release, void)(void* thiz);

		/**
		* Specifies the unique player ID for the current session, such as a player ID, account number, Steam ID, or other
		* number that uniquely identifies the current player. This value must exactly match the value presented to H6AC
		* by your server implementation. If the values mismatch or are invalid, the player will be kicked from the server.
		*
		* This value may be automatically acquired by H6AC.
		*
		* @see H6AC::isPlayerIDAcquired for more details about auto-acquiring
		* @param thiz pointer to the H6AC instance
		* @param playerID a 128-bit integer that represents the current player universally uniquely
		*/
		H6NSDK_VIRTUAL(setPlayerUniqueID, void)(void* thiz, H6N_PlayerID playerID);

		/**
		* Determines if the player unique ID is known to H6AC at the time of calling.
		* H6AC can sometimes figure out the player ID on its' own, such as by calling into the Steamworks or other platform
		* API to retrieve the player ID.  In these cases, there's no need to manually specify the player ID with the
		* H6AC::setPlayerUniqueID function.
		*
		* @param thiz pointer to the H6AC instance
		* @return 1 if the player unique ID has been acquired through any means, 0 if no ID had been established
		*/
		H6NSDK_VIRTUAL(isPlayerIDAquired, int)(void* thiz);

	}_H6NSDK_IFACE_END(H6ACClient, 1);
#define H6ACClient H6NSDK_INTERFACE(H6ACClient, 1)



#define H6AC_SERVER_VERSION 1
#define H6AC_SERVER_INTERFACE "H6ACServer"

	/**
	*
	* Interface name defined in H6AC_INTERFACE as "H6ACServer"
	* Current interface version defined in H6AC_VERSION as 1
	*/
	_H6NSDK_IFACE_BEGIN(H6ACServer, 1) {

		/**
		* Frees the structure.
		* @param thiz pointer to the H6AC instance
		*/
		H6NSDK_VIRTUAL(release, void)(void* thiz);



	} _H6NSDK_IFACE_END(H6ACServer, 1);
#define H6ACServer H6NSDK_INTERFACE(H6ACServer, 1)

#ifdef __cplusplus
}
#endif

#endif //_H6NSDK_INTERFACES_H


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
	void* H6N_createInterface(const char* name, int version);

#ifdef __cplusplus
}
#endif

#endif //_H6NSDK_LIBH6N_H
