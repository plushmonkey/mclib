#ifndef MCLIB_FORGE_H_
#define MCLIB_FORGE_H_

#include "Packets/PacketHandler.h"
#include "Connection.h"
#include <unordered_map>
#include <functional>

namespace Minecraft {
namespace Forge {

class ForgeHandler : public Minecraft::Packets::PacketHandler {
public:
    struct ModInfo {
        std::string name;
        std::string version;

        ModInfo(const std::string& name, const std::string& version) : name(name), version(version) { }
    };

private:
    typedef std::unordered_map<std::wstring, std::function<void(const std::string&)>> HandlerMap;

    HandlerMap m_Handlers;
    // Filled out during ping response
    std::vector<ModInfo> m_Mods;
    Minecraft::Connection* m_Connection;
    u8 m_Version;
    bool m_ModInfoReceived;

    void HandleData(const std::string& data);

    template <typename SerializableData>
    void SendPluginMessage(const std::wstring& channel, SerializableData data) {
        std::string out = data.Serialize().ToString();

        Minecraft::Packets::Outbound::PluginMessagePacket packet(channel, out);
        m_Connection->SendPacket(&packet);
    }

public:
    MCLIB_API ForgeHandler(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Connection* connection);
    MCLIB_API ~ForgeHandler();

    MCLIB_API void HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet);
    MCLIB_API void HandlePacket(Minecraft::Packets::Inbound::Status::ResponsePacket* packet);

    MCLIB_API bool HasModInfo() const;
};

} // ns Forge
} // ns Minecraft

#endif
