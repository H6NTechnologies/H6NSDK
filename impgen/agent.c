//
// Use this instead of dynamically linking via the system so that it doesn't pull in H6Agent unless the game actually needs it
//

#include <libh6n/libh6n.h>


void* Agent_createInterface(const char* name, int version) {
    return (void*)0;
}