#include "gtest/gtest.h"

#include <libh6n/libh6n.h>

class H6NSDKEnvironment : public testing::Environment {
public:
	void SetUp() override {
		H6N_initialize();
	}
};

::testing::Environment* const env = ::testing::AddGlobalTestEnvironment(new H6NSDKEnvironment);



/*
 * Regression testing for the SDK
 */
TEST(SDKAgent, TestNonExistingInterfaceName) {
	EXPECT_EQ(Agent_createInterface("DoesNotExist", 1), H6N_ERROR_INTERFACE_NOT_FOUND);
}

TEST(SDKAgent, TestInvalidInterfaceName) {
	EXPECT_EQ(Agent_createInterface("", 1), H6N_ERROR_INTERFACE_NOT_FOUND);
}

TEST(SDKAgent, TestNonExistingInterfaceVer) {
	EXPECT_EQ(Agent_createInterface(H6AC_CLIENT_INTERFACE, 9001), H6N_ERROR_INTERFACE_NOT_FOUND);
}

TEST(SDKAgent, TestNegativeInterfaceVer) {
	EXPECT_EQ(Agent_createInterface(H6AC_CLIENT_INTERFACE, -1), H6N_ERROR_INTERFACE_NOT_FOUND);
}

TEST(SDKAgent, TestClientCreateVer1) {
	// Test creation
	H6ACClient* cli = (H6ACClient*)Agent_createInterface(H6AC_CLIENT_INTERFACE, 1);
	EXPECT_NE(cli, nullptr);

	// Test that all calls don't crash
	EXPECT_EQ(cli->isPlayerIDAquired(), 0);
	H6N_Int128 id = { 0x1234, 0x1234 };
	cli->setPlayerUniqueID(id);
	cli->submitClientAttestation(nullptr, 0);
	cli->disconnect();
}

TEST(SDKAgent, TestServerCreateVer1) {
	// Test creation
	H6ACServer* serv = (H6ACServer*)Agent_createInterface(H6AC_SERVER_INTERFACE, 1);
	EXPECT_NE(serv, nullptr);

	// Test that all calls don't crash
	H6N_Int128 i;
	serv->registerPlayer(i, 0, 0);
	serv->setAttestationCallback(nullptr);
	serv->setKickCallback(nullptr);
	serv->unregisterPlayer(i);
	serv->begin(i);
	serv->end();
	serv->setUpdateCallback(nullptr);
}

TEST(SDKAgent, TestClientAcquire) {
	EXPECT_NE(Agent_createClient(), nullptr);
}

TEST(SDKAgent, TestServerAcquire) {
	EXPECT_NE(Agent_createServer(), nullptr);
}

