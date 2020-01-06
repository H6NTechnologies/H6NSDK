



# H6NSDK

Take a look at our new [sample project](https://github.com/H6NTechnologies/Sample-AssaultCube/)!

## About This Project

H6NSDK is a collection of code, designed to integrate H6N Technologies into your game (such as implementing H6AC, our flagship anti-cheat).  Specifically included are C/C++ headers, library sources, and build scripts that build the library on a number of targets, i.e., such as bindings for other languages (i.e., C#/.NET).  The contents of this repository areis  source-only.  Customers may access pre-built versions of these projects through the Dev Center, located on our website.

**Why is this repository public?**

At H6N, we do not believe in “security by obscurity -- the security of our software should not depend on keeping secrets from an adversary.  As such, we don’t see any purpose in keeping this project private.  In fact, we believe there are advantages to publishing it, particularly for our customers.  

Please feel free to add this repository to your project as a git submodule, or,  better yet, to incorporate it into your continuous integration system!


## Including H6NSDK In Your Project

Follow these guides for the quickest way to protect your game with H6AC.


### C/C++ Visual Studio Project

**Prerequisites & Requirements**
Todo

### C# Visual Studio Project

**Prerequisites & Requirements**
Todo

### CMake Project

It is’s super easy to use H6NSDK with a CMake-based project.  Because H6NSDK itself is built with CMake, so you can take advantage of the CMake target system to simplify your build scripts.

**Prerequisites & Requirements**



*   An existing C/C++-based project built with CMake version 3.0.0 or newer.

**Instructions**



1. **Download (or compile) the H6NSDK for the target operating system, platform, and architecture from the Dev Center, on our website.** \
 \
The H6NSDK is distributed as a .zip archive, with the following directory structure below.
*   `bin/` contains shared libraries
*   `lib/` contains static libraries
*   `include/` contains the header files for which you should include in your project
2. **Add the extracted H6NSDK archive to your project’s source tree.** 


    You may decide where to place external libraries, depending on your project’s structure.  Leave the SDK’s directory structure intact to take advantage of the included CMake targets.

3. **Add the imported libh6n library to your CMakeLists.txt.**
\
In your CMakeLists: \
`find_package(libh6n CONFIG REQUIRED PATHS path/to/sdk)` \
 \
**NOTE**: Make sure to replace the path with a relative path to the SDK location.

4. **Link your game against libh6n.** \
 \
In your CMakeLists: \
`target_link_libraries(my_game h6nsdk::libh6n)`
5. **Follow the “Generic C/C++ Integration” C/C++ Quick Start Guide (located below).** 



## H6AC Integration Quick-Start


### Generic C/C++ Integration

This guide will demonstrate the basics of how the H6NSDK API works and how to achieve a basic integration in a multiplayer game.

**Prerequisites & Requirements**



*   Your C/C++ project should already be set up, by following the appropriate guide above (“Including H6NSDK In Your Project”).
*   A multiplayer game with a distinct client and authoritative server that can kick arbitrary players.
*   Every player is assigned, _at most_, a 128-bit globally unique player ID, e.g., a Steam ID.

**Instructions**



1. **Include libh6n.h.** \
 \
In both the client and server, include` libh6n/libh6n.h` in every source file you wish to use the API in. \
 \
`#include <libh6n/libh6n.h>`
2. **Create the H6ACClient interface.** \
 \
The H6NSDK uses what we call “interfaces” to expose functions to you.  Interfaces are simply structures of function pointers, which can be invoked just like a function (similar to virtual classes).  Each interface is versioned, as we use structures to maintain backwards compatibility.  Our goal is to never break API compatibility—an old game should never have to update H6NSDK to use a newer version of H6AC. \
 \
On the game client, we need to create an H6ACClient interface by making a call to the following.

    `Agent_createInterface`: \
`H6ACClient* client = (H6ACClient*)Agent_createInterface(H6AC_CLIENT_INTERFACE, H6AC_CLIENT_VERSION)` \
 \
The first argument is a string, which represents the interface requesting creation. The second argument is the requested interface version. \
 \
The macros `H6AC_CLIENT_INTERFACE` and `H6AC_CLIENT_VERSION `are provided for ease of use, allowing you to use the most recent interface version available to you. \
 \
Since `Agent_createInterface` returns a `void*`, we need to cast it to the interface that we would like to use.  In this case, cast it to `H6ACClient`. \
 \
You should treat created interfaces like a singleton.  Note that the memory buffer that backs each interface is “owned” by libh6n, and should not be manually freed.

3. **Pass the player’s unique ID to H6ACClient.** \
 \
Call the function setPlayerUniqueID on H6ACClient to supply the player’s unique ID.  Since we support up to 128-bit Player IDs, then for Player IDs less than 128-bits, we need to construct a 128-bit integer.  This is what your code should appear like, so far. 
 

        H6ACClient* client = (H6ACClient*)Agent_createInterface(H6AC_CLIENT_INTERFACE, H6AC_CLIENT_VERSION); 
        ... 
        H6N_PlayerID pid; 
        pid.of64.lo =  // acquire 64-bit unique ID, such as a Steam ID and pass it in here 
     
        client->setPlayerUniqueID(pid);
    
    Often times, H6AC can automatically acquire the player’s unique ID, if you are using a platform such as Steam.  See Advanced Usage for additional details. [todo]
4. **Create the H6ACServer interface.** \
 \
On the game server, we’ll need to create an H6ACServer interface, in a similar manner as the client: \
`H6ACServer* server = (H6ACServer*)Agent_createInterface(H6AC_SERVER_INTERFACE, H6AC_SERVER_VERSION);`
\
You’ll only need one server interface instance.  It is thread-safe and can be shared between network channels.
5. **Inform H6ACServer of your Integration ID.** \
 \
When your server starts up, call the `begin` function of `H6ACServer` with your Integration ID.  Your Integration ID can be found on the Dev Center, located on our website. 

        H6N_IntegrationID igr;
        igr.bytes = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // put your Integration ID here 
        server->begin(igr);

6. **Provide callbacks to H6AC.** \
 \
H6AC must be able to kick any connected player at any time.  To do this, we must supply H6AC with a function that it can use to kick arbitrary players.

       bool KickPlayer(H6N_PlayerID playerID, const char* reason) { 
            // kick the player from the server here 
            return true; // return false if unable 
       } 
       … 
       server->setKickCallback(KickPlayer); 
   \
H6AC also needs to be able to send a string of bytes from the server to a client.  This string is called the _client attestation token_.  Your server must receive the token and reliably transmit the token to the specified player over the network.  The token must be securely and exclusively sent to only the indicated player (as broadcasting the token to other players would enable players to impersonate each other). 

        void SendAttestation(H6N_PlayerID playerID, uint8_t* attestation, unsigned int length) { 
    	    // send the specified attestation over the network to the specified player -- the specifics of this are up to your game 
        } 
        … 
        server->setAttestationCallback(SendAttestation);

7. **Submit the client attestation token.** \
 \
When a client attestation token is received by the game client from the server, it needs to submit this token to H6ACClient, shown below. \
 \
`client->submitClientAttestation(attestation, attestationLength);`
8. **Register players when they join the server.** \
 \
For H6AC to begin protecting players’ game clients, it must be notified when the player joins the server.  You will need to call the correct `H6ACClient `function, depending on your networking stack—if the player is connected via IPv4, use `registerPlayerIPv4`; if the player is connected via IPv6, use `registerPlayerIPv6`. More details shown below. \
 \
**For IPv4:** \
`server->registerPlayerIPv4(playerUniqueID, remoteAddr, remotePort);` \
 \
 \
**For IPv6:** \
       `H6N_IPV6 addr = ; // populate 128-bit int here representing the player's IPv6 address` \
       `server->registerPlayerIPv6(playerUniqueID, addr, remotePort); `\
\
The first argument is the player’s 128-bit unique ID, which is the same one specified to the H6ACClient.  The second argument represents the player’s IP address, which is a 32-bit integer for IPv4, or a 128-bit integer for IPv6.  The final argument represents the port number of the remote client.  If the port number is not available or is not applicable to your transit protocol, specify a port number of 0.
9. **Un-register players when they leave the server.** \
 \
When a player disconnects from the server, H6AC must know. This allows H6AC to invalidate the player’s anti-cheat session.  You can do this by calling the `unregisterPlayer` function. \
 \
`server->unregisterPlayer(playerUniqueID);` \
 \
If your server crashes or causes players to disconnect without informing H6AC—don’t worry, as H6AC will eventually garbage collect inactive sessions.
10. **Generate a launcher for your game.** \
 \
Todo.
11. **Add binaries to your deployment.** \
 \
You will need to provide a few extra binaries with your game distribution.  Place your generated launcher in the same directory as your game’s main binary, along with `libcapsule.dll` and `libh6n.dll`.   \
 \
**NOTE**: Make sure to update your game’s relevant settings in your distribution platform(s), such as Steam.

12. **Congratulations! Your game is now securely protected by H6AC!** \
 \
Run your game’s new distribution package to ensure your integration is working.
 



### C# Quick Start Guide

TODO


### Integrate with Unity Engine

The H6NSDK has built-in support for the Unity engine, via the C# bindings generator.  Follow this guide to quickly integrate H6AC in your Unity-based game.

TODO


### Integrate with Unreal Engine

TODO


## Building H6NSDK

Firstly, ask yourself whether you really need to build H6NSDK yourself.  You will mostly likely benefit more from using the pre-built version, which is available for download through the Dev Center, located on our website.  Our pre-built binaries are run through an extensive automated test suite, which ensures reliability—we do **not **recommend self-built binaries, as we cannot guarantee support for them (unless you are building for a target that is not pre-built by H6N, or if you have custom toolchain requirements).

**Prerequisites & Requirements**



*   CMake version 3.0.0 or newer. ([Download here](https://cmake.org))
*   [SWIG](http://www.swig.org/) 3.0 or newer, added to your system’s PATH environment variable (if generating non-C bindings, like C#).
*   One of the supported build systems below:
    *   Microsoft Visual Studio 2010 or later (earlier versions are supported, if using a compatible `stdint.h` in your include path)
    *   Unix Makefiles on GCC or Clang
    *   [Ninja](https://ninja-build.org/) (our preferred production build system)
    *   Xcode
*   Checkout (or download) this project (<code>git clone [https://github.com/h6ntechnologies/H6NSDK.git](https://github.com/h6ntechnologies/H6NSDK.git)</code>).

**Instructions**

**• Building with Visual Studio**



1. Open Command Prompt in the H6NSDK directory.
2. Run `cmake -G "Visual Studio X" . `where X is the VS version number you would like to use.  For example, use “Visual Studio 15” to use Visual Studio 2017.
3. Open the `h6nsdk.sln`  file that was generated in the H6NSDK directory.
4. Select the appropriate build configuration for your target in Visual Studio, including your target architecture.
5. Build the solution with Visual Studio.  The output binaries should be listed in the Output window.

**• Building with Unix Makefiles**



1. Navigate your shell to the H6NSDK directory.
2. Run `cmake -G "Unix Makefiles" . && make` .

**• Building with Ninja**



1. If you intend to build on Visual Studio with Ninja, open a Visual Studio Tools Command Prompt or vsvarsall.bat for the appropriate target and VS version.
2. Navigate your shell to the H6NSDK directory.
3. Run `cmake -G "Ninja" . && ninja all`.

**• Building with Xcode**


1. Navigate Terminal to the H6NSDK directory.
2. Run `cmake -G "Xcode" . `.
3. Open the _h6nsdk.xcodeproj _file that was generated in the H6NSDK directory.
4. Select the appropriate build configuration in XCode, including your target architecture.
5. Build the ALL_BUILD target in XCode.  The output binaries should be listed in the Messages screen.


## Project Overview


### Using proxied mode vs direct mode

libh6n offers two modes, which interface with the underlying libraries—we call these modes **proxied mode** and **direct mode**.  Both modes feature their own advantages and disadvantages, so select the appropriate mode, depending on your specific usage needs.

**Proxied mode**



*   We generally recommend using proxied mode.
*   All function calls pass through an intermediary shared library (`libh6n.[dll/so/dylib]`).
*   libh6n lazy-loads the requested underlying library on request, i.e., `libcapsule.dll` will not be loaded until a call to `Capsule_createInterface`.
*   libh6n can hot-reload any underlying library.  For example, if there is an update to H6AC while a player is in-game, H6AC can be automatically updated without having to restart the game.
*   You either have to statically link against libh6n, or include the libh6n dynamic library with your game.  Either way, this will increase your game’s file size by only a few kilobytes.

**Direct mode**



*   Direct mode is easier to get started with.
*   libh6n becomes “header-only.” That is, all function calls pass through directly to the underlying library via dynamic linking.
*   On Windows systems, an implib needs to be generated for each underlying library that you would like to use.  Precompiled implibs are available for download through the Dev Center, located on our website.
*   While it is possible to still hot-swap underlying libraries, it is lot harder—we do not recommend writing your own hot-swapping code.


# Finishing Up

We're excited to support the success of your game.  If you need *any* help at all with H6NSDK or integrating it into your game, please [contact us](https://h6ntechnologies.com/contact-us/business/index.html) anytime to connect directly with our engineers.  


We're always more than happy to help. :)
