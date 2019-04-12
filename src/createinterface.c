//
// Created by brenhein on 4/11/2019.
//

#include <libh6n/libh6n.h>

#define AGENT_CREATE_INTERFACE_FUNCTION "createInterface"

typedef void*(*createInterface_t)(const char* interface, int version);

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>
typedef HANDLE SharedLibraryHandle;

#define AGENT_FILE_NAME "H6Agent.dll"
#define SHARED_LIBRARY_EMPTY_HANDLE 0

SharedLibraryHandle Platform_openLibrary(const char* library) {
    return LoadLibraryA(library);
}

void* Platform_libraryFunctionAddress(SharedLibraryHandle library, const char* functionName) {
    return GetProcAddress(library, functionName);
}

#elif defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>
typedef void* SharedLibraryHandle;
#define SHARED_LIBRARY_EMPTY_HANDLE 0

#ifdef __APPLE__
#define AGENT_FILE_NAME "H6Agent.dylib"
#else
#define AGENT_FILE_NAME "H6Agent.so"
#endif

SharedLibraryHandle Platform_openLibrary(const char* library) {
    return dlopen(library, 0);
}

void* Platform_libraryFunctionAddress(SharedLibraryHandle library, const char* functionName) {
    return dlsym(library, functionName);
}

#endif

SharedLibraryHandle loadOrAcquireAgent() {
    return Platform_openLibrary(AGENT_FILE_NAME);
}

void* createInterface(SharedLibraryHandle handle, const char* iface, int version) {
    // Acquire function pointer to createInterface
    createInterface_t func =
            (createInterface_t)Platform_libraryFunctionAddress(handle,AGENT_CREATE_INTERFACE_FUNCTION);

    if(func == 0)
        return 0;

    // Call createInterface function in agent library
    return func(iface, version);
}

void* H6N_createInterface(const char* name, int version) {
    // First, load/acquire agent shared library
    SharedLibraryHandle agent = loadOrAcquireAgent();

    // Check to make sure load/acquire succeeded
    if(agent == SHARED_LIBRARY_EMPTY_HANDLE)
        return 0; // library couldn't be loaded

    // Call & return result of createInterface call in H6Agent
    return createInterface(agent, name, version);

}