#include "../mclib/Common.h"
#include "../mclib/Client.h"
#include <thread>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/mclib.lib")
#else
#pragma comment(lib, "../Release/mclib.lib")
#endif

int main(void) {
    Minecraft::Packets::PacketDispatcher dispatcher;

    Client client(&dispatcher);

    try {
        client.Login("192.168.2.89", 25565, "testplayer", "");
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
