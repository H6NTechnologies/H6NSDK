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
#define ThisType struct H6NSDK_IFACE_STRUCT(H6ACClient, 1)

    /**
     * Frees the structure.
     * @param thiz pointer to the H6AC instance
     */
	H6NSDK_VIRTUAL(release, void)(ThisType* thiz);

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
    H6NSDK_VIRTUAL(setPlayerUniqueID, void)(ThisType* thiz, H6N_PlayerID playerID);

    /**
     * Determines if the player unique ID is known to H6AC at the time of calling.
     * H6AC can sometimes figure out the player ID on its' own, such as by calling into the Steamworks or other platform
     * API to retrieve the player ID.  In these cases, there's no need to manually specify the player ID with the
     * H6AC::setPlayerUniqueID function.
     *
     * @param thiz pointer to the H6AC instance
     * @return 1 if the player unique ID has been acquired through any means, 0 if no ID had been established
     */
    H6NSDK_VIRTUAL(isPlayerIDAquired, int)(ThisType* thiz);

#undef ThisType
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
