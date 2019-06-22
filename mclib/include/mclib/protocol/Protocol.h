#ifndef MCLIB_PROTOCOL_PROTOCOL_H_
#define MCLIB_PROTOCOL_PROTOCOL_H_

#include <mclib/protocol/ProtocolState.h>
#include <mclib/protocol/packets/Packet.h>
#include <string>
#include <unordered_map>

namespace mc {
namespace protocol {

using PacketCreator = packets::InboundPacket* (*)();
using PacketMap = std::unordered_map<s32, s32>;

class UnsupportedPacketException : public std::exception {
private:
    std::string m_ErrorMessage;

public:
    UnsupportedPacketException(const std::string& message)
        : m_ErrorMessage(message) {}

    virtual const char* what() const noexcept { return m_ErrorMessage.c_str(); }
};

class Protocol {
public:
    typedef std::unordered_map<State, PacketMap> StateMap;

protected:
    StateMap m_InboundMap;
    Version m_Version;

public:
    Protocol(Version version, StateMap inbound)
        : m_InboundMap(inbound), m_Version(version) {}

    virtual ~Protocol() {}

    virtual Version GetVersion() const noexcept { return m_Version; }

    // Creates an inbound packet from state and packet id
    virtual packets::InboundPacket* CreateInboundPacket(State state, s32 id);

    // Convert the protocol id into a protocol agnostic id.
    // This is used as the dispatching id.
    bool GetAgnosticId(State state, s32 protocolId, s32& agnosticId);

    // Handshake
    virtual s32 GetPacketId(packets::out::HandshakePacket) { return 0x00; }

    // Login
    virtual s32 GetPacketId(packets::out::LoginStartPacket) { return 0x00; }
    virtual s32 GetPacketId(packets::out::EncryptionResponsePacket) {
        return 0x01;
    }

    // Status
    virtual s32 GetPacketId(packets::out::status::RequestPacket) {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::status::PingPacket) { return 0x01; }

    // Play
    virtual s32 GetPacketId(packets::out::TeleportConfirmPacket) {
        return 0x00;
    }
    virtual s32 GetPacketId(packets::out::TabCompletePacket) { return 0x01; }
    virtual s32 GetPacketId(packets::out::ChatPacket) { return 0x02; }
    virtual s32 GetPacketId(packets::out::ClientStatusPacket) { return 0x03; }
    virtual s32 GetPacketId(packets::out::ClientSettingsPacket) { return 0x04; }
    virtual s32 GetPacketId(packets::out::ConfirmTransactionPacket) {
        return 0x05;
    }
    virtual s32 GetPacketId(packets::out::EnchantItemPacket) { return 0x06; }
    virtual s32 GetPacketId(packets::out::ClickWindowPacket) { return 0x07; }
    virtual s32 GetPacketId(packets::out::CloseWindowPacket) { return 0x08; }
    virtual s32 GetPacketId(packets::out::PluginMessagePacket) { return 0x09; }
    virtual s32 GetPacketId(packets::out::UseEntityPacket) { return 0x0A; }
    virtual s32 GetPacketId(packets::out::KeepAlivePacket) { return 0x0B; }
    virtual s32 GetPacketId(packets::out::PlayerPositionPacket) { return 0x0C; }
    virtual s32 GetPacketId(packets::out::PlayerPositionAndLookPacket) {
        return 0x0D;
    }
    virtual s32 GetPacketId(packets::out::PlayerLookPacket) { return 0x0E; }
    virtual s32 GetPacketId(packets::out::PlayerPacket) { return 0x0F; }
    virtual s32 GetPacketId(packets::out::VehicleMovePacket) { return 0x10; }
    virtual s32 GetPacketId(packets::out::SteerBoatPacket) { return 0x11; }
    virtual s32 GetPacketId(packets::out::PlayerAbilitiesPacket) {
        return 0x12;
    }
    virtual s32 GetPacketId(packets::out::PlayerDiggingPacket) { return 0x13; }
    virtual s32 GetPacketId(packets::out::EntityActionPacket) { return 0x14; }
    virtual s32 GetPacketId(packets::out::SteerVehiclePacket) { return 0x15; }
    virtual s32 GetPacketId(packets::out::ResourcePackStatusPacket) {
        return 0x16;
    }
    virtual s32 GetPacketId(packets::out::HeldItemChangePacket) { return 0x17; }
    virtual s32 GetPacketId(packets::out::CreativeInventoryActionPacket) {
        return 0x18;
    }
    virtual s32 GetPacketId(packets::out::UpdateSignPacket) { return 0x19; }
    virtual s32 GetPacketId(packets::out::AnimationPacket) { return 0x1A; }
    virtual s32 GetPacketId(packets::out::SpectatePacket) { return 0x1B; }
    virtual s32 GetPacketId(packets::out::PlayerBlockPlacementPacket) {
        return 0x1C;
    }
    virtual s32 GetPacketId(packets::out::UseItemPacket) { return 0x1D; }

    virtual s32 GetPacketId(packets::out::PrepareCraftingGridPacket) {
        throw UnsupportedPacketException(
            "PrepareCraftingGridPacket requires protocol 1.12.0");
    }
    virtual s32 GetPacketId(packets::out::CraftingBookDataPacket) {
        throw UnsupportedPacketException(
            "CraftingBookDataPacket requires protocol 1.12.0");
    }
    virtual s32 GetPacketId(packets::out::AdvancementTabPacket) {
        throw UnsupportedPacketException(
            "AdvancementTabPacket requires protocol 1.12.0");
    }

    virtual s32 GetPacketId(packets::out::CraftRecipeRequestPacket) {
        throw UnsupportedPacketException(
            "CraftRecipeRequestPacket requires protocol 1.12.1");
    }

    static Protocol& GetProtocol(Version version);
};

}  // namespace protocol
}  // namespace mc

#endif
