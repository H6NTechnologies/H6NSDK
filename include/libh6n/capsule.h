#include "libh6n/common.h"


#ifndef _H6NSDK_CAPSULE_H
#define _H6NSDK_CAPSULE_H


#if defined(_WIN32) || defined(_WIN64)
#define _H6N_IMPORT
#define _H6N_EXPORT __declspec(dllexport)
#else
#define _H6N_IMPORT
#define _H6N_EXPORT
#endif

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

	void Capsule_flattenArgs(int argc, char** argv, char* out,
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

	unsigned int Capsule_flattenArgsLength(int argc, char** argv);

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
	* Parameters required by H6Capsule upon launching.
	*/
	typedef struct _CapsuleConfig {
		/**
		*
		*/
		char* targetExecutable;

		/**
		*
		*/
		char* commandLine;

		/**
		*
		*/
		int reportErrors;

		/**
		*
		*/
		Capsule_errorCallback errorCallback;

		/**
		*
		*/
		Capsule_progressCallback progressCallback;


	} CapsuleConfig;

	/**
	* Call this to securely launch the game.  This function serves as the demarcation point between
	* the parent launcher implementation and H6Capsule. This function will always launch the target
	* executable with H6AC loaded &amp; running.
	*
	* @param 	  	integrationID	the H6N Integration ID, acquired from the H6N Dev Center, under
	* 								Manage Integrations.
	* @param [in]	context		 	The context.
	*
	* @return	0 if the operation succeeded. If != 0, an operating system error code.
	*/
	long Capsule_launch(H6N_IntegrationID integrationID, CapsuleConfig context);


#ifdef __cplusplus
}
#endif

#endif