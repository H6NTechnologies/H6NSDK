#include "libh6n/libh6n.h"


/*
 * Platform layer
 *
 * While H6NSDK is internally built inside the monorepo source tree, this library may need to
 * be built by customers in a shared-source agreement. As such, we cannot use the platform
 * abstraction layer in libagent and must re-implement platform-dependent units in here.
 */


#if defined(_WIN32)
#include <Windows.h>
typedef CRITICAL_SECTION PlatformMutex;
#elif defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
typedef pthread_mutex_t PlatformMutex;
#define _H6N_POSIX
#endif


void Platform_initMutex(PlatformMutex* mutex);
void Platform_enterMutex(PlatformMutex* mutex);
void Platform_leaveMutex(PlatformMutex* mutex);

void* Platform_acquireModule(const char* moduleName);
void Platform_freeModule(void* handle);
void* Platform_moduleSymbol(void* handle, const char* symbolName);

/*
 * Global state
 */

typedef void* (_H6N_SPEC* createInterface_t)(const char* name, int version);
typedef void* (_H6N_SPEC* flattenArgs_t)(int argc, char** argv, char* out, unsigned int outLength);
typedef unsigned int (_H6N_SPEC* flattenArgsLength_t)(int argc, char** argv);


typedef struct {
	void* handle;
	createInterface_t createInterface;
	PlatformMutex mutex;
} ModuleState;

typedef struct {
	ModuleState module;
	flattenArgs_t flattenArgs;
	flattenArgsLength_t flattenArgsLen;
} CapsuleState;


ModuleState GAgent = { 0 };
CapsuleState GCapsule = { 0 };


void InitModule(ModuleState& state) {
	Platform_initMutex(&state.mutex);
}

void ReleaseModule(ModuleState& state) {
	Platform_enterMutex(&state.mutex);

	Platform_freeModule(state.handle);
	state.handle = 0;

	Platform_leaveMutex(&state.mutex);
}

bool AcquireModule(ModuleState& state, const char* modulePath, const char* ciName) {
	if (state.handle == 0) {
		state.handle = Platform_acquireModule(modulePath);
		if (state.handle == 0)
			return false;
	}
	state.createInterface = (createInterface_t)Platform_moduleSymbol(state.handle, ciName);
	return state.createInterface != 0;
}

bool AcquireAgent() {
	return AcquireModule(GAgent, H6N_AGENT_MODULE, "Agent_createInterface");
}

bool AcquireCapsule() {
	if (!AcquireModule(GCapsule.module, H6N_CAPSULE_MODULE, "Capsule_createInterface"))
		return false;

	GCapsule.flattenArgs = (flattenArgs_t)Platform_moduleSymbol(GCapsule.module.handle, "Capsule_flattenArgs");
	GCapsule.flattenArgsLen = (flattenArgsLength_t)Platform_moduleSymbol(GCapsule.module.handle, "Capsule_flattenArgsLength");

	return GCapsule.flattenArgs != 0 && GCapsule.flattenArgsLen;
}

/*
 * Utility function implementation
 */
H6ACServer* Agent_createServer() {
	return (H6ACServer*)Agent_createInterface(H6AC_SERVER_INTERFACE, H6AC_SERVER_VERSION);
}

H6ACClient* Agent_createClient() {
	return (H6ACClient*)Agent_createInterface(H6AC_CLIENT_INTERFACE, H6AC_CLIENT_VERSION);
}

H6Capsule* Capsule_createCapsule() {
	return (H6Capsule*)Capsule_createInterface(H6N_CAPSULE_INTERFACE, H6N_CAPSULE_VERSION);
}

H6N_Int128 H6N_createInt128(uint64_t low, uint64_t high) {
	return { low, high };
}

/*
 * Exported function implementation
 */

extern "C" {

	void H6N_initialize() {
		InitModule(GAgent);
		InitModule(GCapsule.module);
	}

	void Agent_release() {
		ReleaseModule(GAgent);
	}

	void Capsule_release() {
		ReleaseModule(GCapsule.module);
	}

	void* _H6N_SPEC Agent_createInterface(const char* name, int version) {
		Platform_enterMutex(&GAgent.mutex);

		if (!AcquireAgent()) {
			Platform_leaveMutex(&GAgent.mutex);
			return 0;
		}

		void* result = GAgent.createInterface(name, version);
		Platform_leaveMutex(&GAgent.mutex);
		return result;
	}

	void* _H6N_SPEC Capsule_createInterface(const char* name, int version) {
		Platform_enterMutex(&GCapsule.module.mutex);

		if (!AcquireCapsule()) {
			Platform_leaveMutex(&GCapsule.module.mutex);
			return 0;
		}

		void* result = GCapsule.module.createInterface(name, version);
		Platform_leaveMutex(&GCapsule.module.mutex);
		return result;
	}

	void _H6N_SPEC Capsule_flattenArgs(int argc, char** argv, char* out, unsigned int outLength) {
		Platform_enterMutex(&GCapsule.module.mutex);

		if (!AcquireCapsule()) {
			Platform_leaveMutex(&GCapsule.module.mutex);
			return;
		}

		GCapsule.flattenArgs(argc, argv, out, outLength);
	}


	unsigned int _H6N_SPEC Capsule_flattenArgsLength(int argc, char** argv) {
		Platform_enterMutex(&GCapsule.module.mutex);

		if (!AcquireCapsule()) {
			Platform_leaveMutex(&GCapsule.module.mutex);
			return 0;
		}

		int result = GCapsule.flattenArgsLen(argc, argv);
		Platform_leaveMutex(&GCapsule.module.mutex);
		return result;
	}

}



#if defined(_WIN32)

void Platform_initMutex(PlatformMutex* mutex) {
	InitializeCriticalSection(mutex);
}

void Platform_enterMutex(PlatformMutex* mutex) {
	EnterCriticalSection(mutex);
}

void Platform_leaveMutex(PlatformMutex* mutex) {
	LeaveCriticalSection(mutex);
}

void* Platform_acquireModule(const char* moduleName) {
	return LoadLibraryA(moduleName);
}

void Platform_freeModule(void* handle) {
	FreeLibrary((HMODULE)handle);
}

void* Platform_moduleSymbol(void* handle, const char* symbolName) {
	return (void*)GetProcAddress((HMODULE)handle, symbolName);
}

#elif defined(_H6N_POSIX)

#include <dlfcn.h>

void Platform_initMutex(PlatformMutex* mutex) {
    pthread_mutex_init(mutex, 0);
}

void Platform_enterMutex(PlatformMutex* mutex) {
	pthread_mutex_lock(mutex);
}

void Platform_leaveMutex(PlatformMutex* mutex) {
	pthread_mutex_unlock(mutex);
}

void* Platform_acquireModule(const char* moduleName) {
    return dlopen(moduleName, 0);
}

void Platform_freeModule(void* handle) {
    dlclose(handle);
}

void* Platform_moduleSymbol(void* handle, const char* symbolName) {
    return (void*)dlsym(handle, symbolName);
}

#endif
