#ifndef CLIENT_BLOCKPLACER_H_
#define CLIENT_BLOCKPLACER_H_

#include <mclib/core/Client.h>
#include <mclib/protocol/packets/PacketHandler.h>
#include <mclib/world/World.h>

namespace example {

class BlockPlacer : public mc::protocol::packets::PacketHandler,
                    public mc::world::WorldListener,
                    public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::util::PlayerController* m_PlayerController;
    mc::world::World* m_World;
    mc::Vector3i m_Target;
    s64 m_LastUpdate;
    mc::inventory::Slot m_HeldItem;

public:
    BlockPlacer(mc::protocol::packets::PacketDispatcher* dispatcher,
                mc::core::Client* client, mc::util::PlayerController* pc,
                mc::world::World* world);
    ~BlockPlacer();

    void HandlePacket(mc::protocol::packets::in::WindowItemsPacket* packet);
    void HandlePacket(mc::protocol::packets::in::SetSlotPacket* packet);

    void OnTick();
};

}  // namespace example

#endif
