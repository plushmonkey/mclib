#ifndef MCLIB_UTIL_FORGE_H_
#define MCLIB_UTIL_FORGE_H_

#include <mclib/core/Connection.h>
#include <mclib/protocol/packets/PacketHandler.h>

#include <functional>
#include <unordered_map>

namespace mc {
namespace util {

class ForgeHandler : public protocol::packets::PacketHandler {
public:
    struct ModInfo {
        std::string name;
        std::string version;

        ModInfo(const std::string& name, const std::string& version)
            : name(name), version(version) {}
    };

private:
    typedef std::unordered_map<std::wstring,
                               std::function<void(const std::string&)>>
        HandlerMap;

    HandlerMap m_Handlers;
    // Filled out during ping response
    std::vector<ModInfo> m_Mods;
    core::Connection* m_Connection;
    u8 m_Version;
    bool m_ModInfoReceived;

    void HandleData(const std::string& data);

    template <typename SerializableData>
    void SendPluginMessage(const std::wstring& channel, SerializableData data) {
        std::string out = data.Serialize().ToString();

        protocol::packets::out::PluginMessagePacket packet(channel, out);
        m_Connection->SendPacket(&packet);
    }

public:
    MCLIB_API ForgeHandler(protocol::packets::PacketDispatcher* dispatcher,
                           core::Connection* connection);
    MCLIB_API ~ForgeHandler();

    MCLIB_API void SetConnection(core::Connection* conn) {
        m_Connection = conn;
    }

    MCLIB_API void HandlePacket(
        protocol::packets::in::PluginMessagePacket* packet);
    MCLIB_API void HandlePacket(
        protocol::packets::in::status::ResponsePacket* packet);

    MCLIB_API bool HasModInfo() const;
};

}  // namespace util
}  // namespace mc

#endif
