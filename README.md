# H6NSDK

Take a look at our new [sample project](https://github.com/H6NTechnologies/Sample-AssaultCube/)!


## Introduction

`H6NSDK` is H6N Technologies' software development kit serving as your interface to H6AC.

The project's primary goals include:
 - As portable as possible
 - Feature complete and well-tested
 - Bindings for many languages
 - Backwards-compatibility
 - All while being easy-to-use

## Code example

This sample is the absolute bare minimum required to securely implement H6AC in your game.

```
H6N_Initialize();

/* In your client */

H6ACClient* client = Agent_createClient();
client->setPlayerUniqueID(/* unique player ID */);
h6acclient->setSharedSecret(/* shared secret value shared between server and client */, len);

/* In your server */

H6ACServer* server = Agent_createServer();
H6N_IntegrationID igrID = H6N_createInt128(/* your game's integration ID */);
server->begin(igrID);
server->setKickCallback(kickCallbackH6AC);

int kickCallbackH6AC(H6N_PlayerID playerID, const char* reason) {
    /* kick the specified player with reason */
}

/* When a player joins the server */

server->registerPlayer(/* player ID */, /* shared secret */, len);

```

**Notes**
 - The player ID is an arbitrary, globally-unique, at-most 128-bit integer assigned to a specific player.  An example of this would be a Steam ID.
 - The shared secret value is an arbitrary value that is known by both the client and the server.  Authentication tokens, encryption keys, or even a random number can be used.


## Additional documentation
 - [Background info](docs/BACKGROUND.md)
 - [Building H6NSDK](docs/BUILDING.md)
 - [Usage guide](docs/USAGE.md)
 - [Additional features](docs/FEATURES.md)


## Getting help


We're excited to support the success of your game.  If you need any help at all with H6NSDK or integrating it into your game, please [contact us](https://h6ntechnologies.com/contact-us/business/index.html) anytime to connect directly with our engineers.  


We're always more than happy to help. :)

