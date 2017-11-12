#include "Logger.h"

#include <mclib/common/Common.h>
#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>
#include <mclib/util/Hash.h>
#include <mclib/util/Utility.h>
#include <mclib/util/VersionFetcher.h>

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclibd.lib")
#pragma comment(lib, "../lib/jsoncpp/lib/jsoncppd-msvc-2017.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#pragma comment(lib, "../lib/jsoncpp/lib/jsoncpp-msvc-2017.lib")
#endif

namespace {

const std::string server("127.0.0.1");
const u16 port = 25565;
const std::string username("testplayer");
const std::string password("");

} // ns

int run(mc::protocol::Version versions, mc::util::ForgeHandler& forge);

int main(void) {
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks();
    mc::util::VersionFetcher versionFetcher(server, port);

    std::cout << "Fetching version" << std::endl;

    auto version = versionFetcher.GetVersion();

    std::cout << "Connecting with version " << mc::protocol::to_string(version) << std::endl;
    return run(version, versionFetcher.GetForge());
}

int run(mc::protocol::Version version, mc::util::ForgeHandler& forge) {
    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::core::Client client(&dispatcher, version);

    forge.SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
        .SetMainHand(mc::MainHand::Right)
        .SetViewDistance(16);

    example::Logger logger(&client, &dispatcher);

    try {
        std::cout << "Logging in." << std::endl;
        client.Login(server, port, username, password, mc::core::UpdateMethod::Block);
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
