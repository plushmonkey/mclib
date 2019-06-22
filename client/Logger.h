#ifndef CLIENT_LOGGER_H_
#define CLIENT_LOGGER_H_

#include <mclib/core/Client.h>
#include <mclib/protocol/packets/PacketHandler.h>

namespace example {

class Logger : public mc::protocol::packets::PacketHandler,
               public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;

public:
    Logger(mc::core::Client* client,
           mc::protocol::packets::PacketDispatcher* dispatcher);
    ~Logger();

    void HandlePacket(mc::protocol::packets::in::ChatPacket* packet) override;
    void HandlePacket(
        mc::protocol::packets::in::EntityLookAndRelativeMovePacket* packet)
        override;
    void HandlePacket(
        mc::protocol::packets::in::BlockChangePacket* packet) override;
    void HandlePacket(
        mc::protocol::packets::in::MultiBlockChangePacket* packet) override;
    void HandlePacket(
        mc::protocol::packets::in::DisconnectPacket* packet) override;

    void OnTick() override;
};

}  // namespace example

#endif
