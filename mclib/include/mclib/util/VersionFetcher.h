#ifndef _MCLIB_UTIL_VERSIONFETCHER_H_
#define _MCLIB_UTIL_VERSIONFETCHER_H_

#include <mclib/core/Client.h>
#include <mclib/util/Forge.h>

namespace mc {
namespace util {

class VersionFetcher : public mc::core::ConnectionListener {
private:
    mc::protocol::packets::PacketDispatcher m_Dispatcher;
    mc::protocol::Version m_Version;
    mc::util::ForgeHandler m_Forge;
    mc::core::Connection* m_Connection;
    std::string m_Host;
    u16 m_Port;
    bool m_Found;

public:
    MCLIB_API VersionFetcher(const std::string& host, u16 port);

    MCLIB_API mc::protocol::Version GetVersion();
    MCLIB_API void OnPingResponse(const Json::Value& node) override;

    mc::util::ForgeHandler& GetForge() { return m_Forge; }
};

} // ns util
} // ns mc

#endif
