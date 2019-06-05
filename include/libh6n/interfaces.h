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

#define H6N_AGENT_MODULE "H6Agent" _H6N_SHARED_LIB_EXT


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
 * Interface name defined in H6AC_CLIENT_INTERFACE as "H6ACClient"
 * Current interface version defined in H6AC_CLIENT_VERSION as 1
 */
_H6NSDK_IFACE_BEGIN(H6ACClient, 1) {

    /**
     * Specifies the unique player ID for the current session, such as a player ID, account number, Steam ID, or other
     * number that uniquely identifies the current player. This value must exactly match the value presented to H6AC
     * by your server implementation. If the values mismatch or are invalid, the player will be kicked from the server.
     *
     * This value may be automatically acquired by H6AC.
     *
     * @see H6AC::isPlayerIDAcquired for more details about auto-acquiring
     * @param playerID a 128-bit integer that represents the current player universally uniquely
     */
    H6NSDK_VIRTUAL(setPlayerUniqueID, void)(H6N_PlayerID playerID);

    /**
     * Determines if the player unique ID is known to H6AC at the time of calling.
     * H6AC can sometimes figure out the player ID on its' own, such as by calling into the Steamworks or other platform
     * API to retrieve the player ID.  In these cases, there's no need to manually specify the player ID with the
     * H6AC::setPlayerUniqueID function.
     *
     * @return 1 if the player unique ID has been acquired through any means, 0 if no ID had been established
     */
    H6NSDK_VIRTUAL(isPlayerIDAquired, int)();

	/**
	 * Submits an acquired client attestation token to the agent.
	 * Attestation tokens provide a strongly secure method to authenticate a client. Without an attestation token, the H6AC client
	 * will present only the player unique ID to the H6AC server for authentication. While we believe this is "good enough" security,
	 * you may elect to enable client authentication tokens, in which your game's server sends an arbitrarily-lengthed token, generated
	 * by H6ACServer, to the game client, which then presents the token to the H6AC client using this function.
	 * 
	 * Electing to use client attestation ensures that an attacker cannot maliciously impersonate another player that happens to share the
	 * same network as the attacker.  We find this to be incredibly unlikely, if not impossible on IPv6 networks, but plausible on
	 * IPv4 networks, particularly when the player is behind NAT.
	 *
	 * @param attestation the attestation token to submit, of the specified length
	 * @param length the length of the attestation string, in bytes
	 */
	H6NSDK_VIRTUAL(submitClientAttestation, void)(uint8_t* attestation, unsigned int length);

	/**
	 * Resets the client's internal state. This will manually clear out any session-specific state data being held by
	 * H6AC, such as the client authentication token.  The use case for this is to prevent H6AC from using stale data
	 * to connect to a new server.  This should therefore be typically called upon disconnecting from the current game
	 * server.  Calling this function is optional -- H6AC should eventually clear out its internal state, but this 
	 * function guarantees it.
	 */
	H6NSDK_VIRTUAL(purgeState, void)();



}_H6NSDK_IFACE_END(H6ACClient, 1);
#define H6ACClient H6NSDK_INTERFACE(H6ACClient, 1)

#define H6N_createCient() (H6ACClient*)Agent_createInterface(H6AC_CLIENT_INTERFACE, H6AC_CLIENT_VERSION)


#define H6AC_SERVER_VERSION 1
#define H6AC_SERVER_INTERFACE "H6ACServer"






/**
 * `H6ACServer` is the interface that allows (typically headless) game servers to interact with H6AC, the H6N
 * Technologies Anti-Cheat. Using this interface is *necessary* for a secure implementation of H6AC in your game,
 * as H6AC needs to be notified when a player joins and be able to kick players arbitrarily.
 * 
 * Interface name defined in H6AC_INTERFACE as "H6ACServer"
 * Current interface version defined in H6AC_SERVER_VERSION as 1
 */
_H6NSDK_IFACE_BEGIN(H6ACServer, 1) {

	/**
	 * 
	 */
	H6NSDK_VIRTUAL(registerPlayerIPv4, void)(H6N_PlayerID playerID, long remoteAddr, uint16_t remotePort);

	H6NSDK_VIRTUAL(registerPlayerIPv6, void)(H6N_PlayerID playerID, H6N_IPV6 remoteAddr, uint16_t remotePort);

	/**
	 * Informs H6AC that the player should no longer be considered by H6AC. Typically, this is called when the player
	 * disconnects.  
	 */
	H6NSDK_VIRTUAL(unregisterPlayer, void)(H6N_PlayerID playerID);


} _H6NSDK_IFACE_END(H6ACServer, 1);
#define H6ACServer H6NSDK_INTERFACE(H6ACServer, 1)

#define H6N_createServer() (H6ACServer*)Agent_createInterface(H6AC_SERVER_INTERFACE, H6AC_SERVER_VERSION)



#ifdef __cplusplus
}
#endif

#endif //_H6NSDK_INTERFACES_H
