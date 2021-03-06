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
	 * Sets a shared secret value that is unique to the player. This value must be known by the server
	 * before the server registers this client. Examples of acceptable shared secrets include the client's
	 * IP address and port combo, authentication tokens (i.e. Steam auth tokens), shared encryption keys,
	 * etc.  Really anything that is unique to this client session can be used.
	 *
	 * Internally, the shared secret is strongly cryptographically hashed, so secrets of any length are
	 * acceptable.
	 *
	 * @param sharedSecret a unique-to-this-player string
	 * @param sharedSecretLen the length of sharedSecret, in bytes
	 */
	H6NSDK_VIRTUAL(setSharedSecret, void)(const uint8_t* sharedSecret, unsigned int sharedSecretLen);

	/**
	 * Submits an acquired client attestation token to the agent.
	 * Attestation tokens provide a strongly secure method to authenticate a client. Without an attestation token, the H6AC client
	 * will present only the player unique ID + shared secret to the H6AC server for authentication. While we believe this is "good enough" security,
	 * you may elect to enable client authentication tokens, in which your game's server sends an arbitrarily-lengthed token, generated
	 * by H6ACServer, to the game client, which then presents the token to the H6AC client using this function.
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
	H6NSDK_VIRTUAL(disconnect, void)();



}_H6NSDK_IFACE_END(H6ACClient, 1);
#define H6ACClient H6NSDK_INTERFACE(H6ACClient, 1)

H6ACClient* Agent_createClient();




#define H6AC_SERVER_VERSION 1
#define H6AC_SERVER_INTERFACE "H6ACServer"


/**
 * 
 */
typedef int(*H6NSDK_INTERFACE(H6ACServer_kickCallback, 1))(H6N_PlayerID playerID, const char* reason);

/**
 *
 */
typedef void(*H6NSDK_INTERFACE(H6ACServer_attestationCallback, 1))(H6N_PlayerID playerID, uint8_t* attestation, unsigned int length);

/**
 * 
 */
typedef void(*H6NSDK_INTERFACE(H6ACServer_updateCallback, 1))();


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
	H6NSDK_VIRTUAL(begin, void)(H6N_IntegrationID integrationID);

	/**
	 * 
	 */
	H6NSDK_VIRTUAL(end, void)();

	/**
	 * 
	 */
	H6NSDK_VIRTUAL(registerPlayer, void)(H6N_PlayerID playerID, const uint8_t* sharedSecret, unsigned int sharedSecretLen);

	/**
	 * Informs H6AC that the player should no longer be considered by H6AC. Typically, this is called when the player
	 * disconnects.  
	 */
	H6NSDK_VIRTUAL(unregisterPlayer, void)(H6N_PlayerID playerID);

	/**
	 *
	 */
	H6NSDK_VIRTUAL(setKickCallback, void)(H6NSDK_INTERFACE(H6ACServer_kickCallback, 1) callback);

	/**
	 * 
	 */
	H6NSDK_VIRTUAL(setAttestationCallback, void)(H6NSDK_INTERFACE(H6ACServer_attestationCallback, 1) callback);

	/**
	 * 
	 */
	H6NSDK_VIRTUAL(setUpdateCallback, void)(H6NSDK_INTERFACE(H6ACServer_updateCallback, 1) callback);


} _H6NSDK_IFACE_END(H6ACServer, 1);
#define H6ACServer H6NSDK_INTERFACE(H6ACServer, 1)

H6ACServer* Agent_createServer();


#define H6AC_REPORT_VERSION 1
#define H6AC_REPORT_INTERFACE "H6ACReport"

/**
 * `H6ACReport` is an interface through which players can report other players
 * as cheaters.  This interface can be used by your game client or server,
 * whichever you prefer.
 * 
 * Interface name defined in H6AC_REPORT_INTERFACE as "H6ACReport"
 * Current interface version defined in H6AC_REPORT_VERSION as 1
 */
_H6NSDK_IFACE_BEGIN(H6ACReport, 1) {

	/**
	 * Report a player as a cheater, by player ID.
	 *
	 * @param playerID the player ID to report
	 * @param reserved reserved for future use, implementations should set this to zero
	 */
	H6NSDK_VIRTUAL(reportPlayer, void)(H6N_PlayerID playerID, int reserved);


} _H6NSDK_IFACE_END(H6ACReport, 1);
#define H6ACReport H6NSDK_INTERFACE(H6ACReport, 1)

H6ACReport* Agent_createReport();


#ifdef __cplusplus
}
#endif

#endif //_H6NSDK_INTERFACES_H
