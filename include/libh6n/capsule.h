#include <libh6n/common.h>

#ifndef _H6NSDK_CAPSULE_H
#define _H6NSDK_CAPSULE_H


#define H6N_CAPSULE_MODULE "libcapsule" _H6N_SHARED_LIB_EXT

#define _H6N_CAPSULE_RESULT(VAL) ((long)VAL) 
#define H6N_CAPSULE_RESULT_SUCCESS _H6N_CAPSULE_RESULT(1)
#define H6N_CAPSULE_RESULT_RELAUNCH _H6N_CAPSULE_RESULT(-1)
#define H6N_CAPSULE_RESULT_FAILURE _H6N_CAPSULE_RESULT(0)

#ifdef __cplusplus
extern "C" {
#endif


	/**
	* Flattens the arguments in {@code int main(int, char**)} style into a single, space delineated
	* string.  The first element in <pre>argv</pre> is ignored, as it normally contains the path to
	* the executable.
	*
	* @param 	   	argc	 	The number of strings passed in by argv.
	* @param [in] 	argv	 	An array of strings, the arguments to flatten.
	* @param [out]	out		 	The output string buffer to write the flattened args to.
	* @param 	   	outLength	The size of the output buffer, in bytes.
	*/

	_H6N_EXPORTED void _H6N_SPEC Capsule_flattenArgs(int argc, char** argv, char* out,
		unsigned int outLength);

	/**
	* Calculates the buffer size required to store the result of {@link H6Capsule_flattenArgs(int,
	* char**, char*, unsigned int}.
	*
	* @param 	  	argc	The number of strings passed in by argv.
	* @param [in]	argv	An array of strings, with {@code argv} length.
	*
	* @return	The size of the buffer required to hold the flattened string, in bytes.
	*/

	_H6N_EXPORTED unsigned int _H6N_SPEC Capsule_flattenArgsLength(int argc, char** argv);

	/**
	* Callback function that receives an error message.
	*
	* @param	message	The error message.
	*/
	typedef void(__cdecl *Capsule_errorCallback)(const char* message);

	/**
	* Callback function that receives a process percentage.
	*
	* @param	percent	The percent complete, expressed as a float on the interval [0, 1], where
	* 					1 is 100%.
	*/

	typedef void(__cdecl *Capsule_progressCallback)(float percent);


	/**
	 * When this callback is called, it is expected that the launcher will either terminate and reopen,
	 * or unload and reload libcapsule.
	 * 
	 * 
	 */
	typedef void(_H6N_SPEC* Capsule_reloadCallback)();

#define H6N_CAPSULE_VERSION 1
#define H6N_CAPSULE_INTERFACE "H6Capsule"



	_H6NSDK_IFACE_BEGIN(H6Capsule, 1) {
		H6NSDK_VIRTUAL(launch, long)(const char* targetProcess, H6N_IntegrationID id, char* args);
		H6NSDK_VIRTUAL(errorCallback, void)(Capsule_errorCallback errorCallback);
		H6NSDK_VIRTUAL(progressCallback, void)(Capsule_progressCallback progressCallback);
		H6NSDK_VIRTUAL(reloadCallback, void)(Capsule_reloadCallback reloadCallback);
	} _H6NSDK_IFACE_END(H6Capsule, 1);
#define H6Capsule H6NSDK_INTERFACE(H6Capsule, 1)



#ifdef __cplusplus
}
#endif

#endif