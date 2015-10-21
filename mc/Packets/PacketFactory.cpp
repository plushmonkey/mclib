#include "PacketFactory.h"
#include <exception>
#include <string>

namespace Minecraft {
namespace Packets {


class LoginPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

class PlayPacketFactory {
public:
    static Packet* CreatePacket(DataBuffer& data, std::size_t length);
};

Packet* LoginPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;
    u8 id;
    data >> id;

    InboundPacket* packet = nullptr;

    switch (id) {
    case 0x00:
        packet = new Inbound::DisconnectPacket();
        break;
    case 0x01:
        packet = new Inbound::EncryptionRequestPacket();
        break;
    case 0x02:
        packet = new Inbound::LoginSuccessPacket();
        break;
    case 0x03:
        packet = new Inbound::SetCompressionPacket();
        break;
    default:
        throw std::runtime_error("Unknown packet type " + std::to_string(id) + " received during login protocol state.");
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PlayPacketFactory::CreatePacket(DataBuffer& data, std::size_t length) {
    if (data.GetSize() == 0) return nullptr;
    u8 id;
    data >> id;

    InboundPacket* packet = nullptr;

    switch (id) {
    case 0x00:
        packet = new Inbound::KeepAlivePacket();
        break;
    case 0x01:
        packet = new Inbound::JoinGamePacket();
        break;
    case 0x02:
        packet = new Inbound::ChatPacket();
        break;
    case 0x03:
        //packet = new Inbound::TimeUpdatePacket();
        break;
    case 0x05:
        packet = new Inbound::SpawnPositionPacket();
        break;
    case 0x08:
        packet = new Inbound::PlayerPositionAndLookPacket();
        break;
    case 0x09:
        packet = new Inbound::HeldItemChangePacket();
        break;
    case 0x0F:
        packet = new Inbound::SpawnMobPacket();
        break;
    case 0x1C:
        packet = new Inbound::EntityMetadataPacket();
        break;
    case 0x26:
        packet = new Inbound::MapChunkBulkPacket();
        break;
    case 0x2F:
        packet = new Inbound::SetSlotPacket();
        break;
    case 0x30:
        packet = new Inbound::WindowItemsPacket();
        break;
    case 0x37:
        packet = new Inbound::StatisticsPacket();
        break;
    case 0x38:
        packet = new Inbound::PlayerListItemPacket();
        break;
    case 0x39:
        packet = new Inbound::PlayerAbilitiesPacket();
        break;
    case 0x3F:
        packet = new Inbound::PluginMessagePacket();
        break;
    case 0x41:
        packet = new Inbound::ServerDifficultyPacket();
        break;
    case 0x44:
        packet = new Inbound::WorldBorderPacket();
        break;
    default:
        throw std::runtime_error("Unknown packet type " + std::to_string(id) + " received during play protocol state.");
    }

    if (packet)
        packet->Deserialize(data, length);

    return packet;
}

Packet* PacketFactory::CreatePacket(ProtocolState state, DataBuffer& data, std::size_t length) {
    switch (state) {
    case Handshake:
        throw new std::runtime_error("Packet received during handshake (wrong protocol state).");
    case Play:
        return PlayPacketFactory::CreatePacket(data, length);
    case Login:
        return LoginPacketFactory::CreatePacket(data, length);
    default:
        throw new std::runtime_error("Protocol isn't in a valid state.");
    }
}

} // ns Packets
} // ns Minecraft
