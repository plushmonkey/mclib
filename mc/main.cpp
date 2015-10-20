#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <vector>
#include <json/json.h>
#include <openssl/sha.h>
#include "HTTPClient.h"
#include "Hash.h"
#include "Yggdrasil.h"
#include "Network/Network.h"
#include "DataBuffer.h"
#include "Encryption.h"
#include "Packets/Packet.h"
#include "Packets/PacketHandler.h"
#include "Packets/PacketDispatcher.h"
#include "Packets/PacketFactory.h"
#include "Protocol.h"
#include "Compression.h"
#include <memory>

std::string PlayerName = "plushmonkey";
std::string PlayerPassword = "";

class Connection : public Minecraft::Packets::PacketHandler {
private:
    Minecraft::Packets::PacketDispatcher m_Dispatcher;
    Minecraft::EncryptionStrategy* m_Encrypter;
    Minecraft::CompressionStrategy* m_Compressor;
    Minecraft::ProtocolState m_ProtocolState;
    std::shared_ptr<Network::Socket> m_Socket;
    Minecraft::Yggdrasil m_Yggdrasil;
    std::string m_Server;
    u16 m_Port;

public:
    Connection(const std::string& server, u16 port)
        : m_Dispatcher(), 
          m_Server(server), 
          m_Port(port), 
          m_Encrypter(new Minecraft::EncryptionStrategyNone()), 
          m_Compressor(new Minecraft::CompressionNone()),
          m_Socket(new Network::TCPSocket()), 
          Minecraft::Packets::PacketHandler(&m_Dispatcher)
    {
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Login, 0x00, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Login, 0x01, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Login, 0x02, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Login, 0x03, this);

        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x01, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x05, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x08, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x0F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x26, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x2F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x30, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x37, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x38, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x39, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x3F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x41, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x44, this);
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnMobPacket* packet) {
        const auto& metadata = packet->GetMetadata();
        
        std::cout << "Received SpawnMobPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::MapChunkBulkPacket* packet) {
        std::cout << "Received MapChunkBulkPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
        Minecraft::Slot slot = packet->GetSlot();
        int window = packet->GetWindowId();
        int index = packet->GetSlotIndex();

        std::cout << "Set slot (" << window << ", " << index << ") = " << slot.GetItemId() << "\n";
    }

    void HandlePacket(Minecraft::Packets::Inbound::WindowItemsPacket* packet) {
        std::cout << "Received window items for WindowId " << (int)packet->GetWindowId() << "." << std::endl;

        const std::vector<Minecraft::Slot>& slots = packet->GetSlots();
        
        for (const Minecraft::Slot& slot : slots) {
            s16 id = slot.GetItemId();
            u8 count = slot.GetItemCount();
            s16 dmg = slot.GetItemDamage();
            const Minecraft::NBT::NBT& nbt = slot.GetNBT();

            if (id != -1) {
                std::cout << "Item: " << id << " Amount: " << (int)count << " Dmg: " << dmg << std::endl;
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::WorldBorderPacket* packet) {
        using namespace Minecraft::Packets::Inbound;
        WorldBorderPacket::Action action = packet->GetAction();

        std::cout << "Received world border packet\n";

        switch (action) {
            case WorldBorderPacket::Action::Initialize:
            {
                std::cout << "World border radius: " << packet->GetRadius() << std::endl;
                std::cout << "World border center: " << packet->GetX() << ", " << packet->GetZ() << std::endl;
                std::cout << "World border warning time: " << packet->GetWarningTime() << " seconds , blocks: " << packet->GetWarningBlocks() << " meters" << std::endl;
            }
            break;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
        s32 x = (s32)packet->GetX();
        s32 y = (s32)packet->GetY(); 
        s32 z = (s32)packet->GetZ();
        Minecraft::Position pos(x, y , z);

        std::cout << "Pos: " << pos << std::endl;
    }


    void HandlePacket(Minecraft::Packets::Inbound::PlayerListItemPacket* packet) {
        using namespace Minecraft::Packets::Inbound;

        PlayerListItemPacket::Action action = packet->GetAction();

        std::vector<PlayerListItemPacket::ActionDataPtr> dataList = packet->GetActionData();

        for (auto actionData : dataList) {
            switch (action) {
            case PlayerListItemPacket::Action::AddPlayer:
                std::wcout << "Adding player " << actionData->name << " uuid: " << actionData->uuid << std::endl;

                for (auto& property : actionData->properties)
                    std::wcout << property.first << " = " << property.second << std::endl;
            break;
            case PlayerListItemPacket::Action::UpdateGamemode:
                std::wcout << "Updating gamemode to " << actionData->gamemode << " for player " << actionData->uuid << std::endl;
            break;
            case PlayerListItemPacket::Action::UpdateLatency:
                std::wcout << "Ping for " << actionData->uuid << " is " << actionData->ping << std::endl;
            break;
            case PlayerListItemPacket::Action::RemovePlayer:
                std::cout << "Removing player " << actionData->uuid << std::endl;
            break;
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::StatisticsPacket* packet) {
        const auto& stats = packet->GetStatistics();

        for (auto& kv : stats)
            std::wcout << kv.first << " = " << kv.second << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerAbilitiesPacket* packet) {
        std::cout << "Abilities: " << (int)packet->GetFlags() << ", " << packet->GetFlyingSpeed() << ", " << packet->GetWalkingSpeed() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPositionPacket* packet) {
        std::cout << "Spawn position: " << packet->GetLocation() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::DisconnectPacket* packet) {
        std::cout << "Disconnected from server. Reason: " << std::endl;
        std::wcout << packet->GetReason() << std::endl;
        std::abort();
    }

    void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey) {
        std::cout << "Doing client auth\n";
        
        try {
            if (!m_Yggdrasil.Authenticate(PlayerName, PlayerPassword)) {
                std::cerr << "Failed to authenticate" << std::endl;
                std::abort();
            }
        } catch (const Minecraft::YggdrasilException& e) {
            std::cerr << e.what() << std::endl;
            std::abort();
        }

        try {
            if (!m_Yggdrasil.JoinServer(serverId, sharedSecret, pubkey)) {
                std::cerr << "Failed to join server through Yggdrasil." << std::endl;
                std::abort();
            }
        } catch (const Minecraft::YggdrasilException& e) {
            std::cerr << e.what() << std::endl;
            std::abort();
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::EncryptionRequestPacket* packet) {
        std::string pubkey = packet->GetPublicKey();
        std::string verify = packet->GetVerifyToken();

        std::cout << "Encryption request received\n";

        Minecraft::EncryptionStrategyAES* aesEncrypter = new Minecraft::EncryptionStrategyAES(pubkey, verify);
        Minecraft::Packets::Outbound::EncryptionResponsePacket* encResp = aesEncrypter->GenerateResponsePacket();

        AuthenticateClient(packet->GetServerId().GetUTF16(), aesEncrypter->GetSharedSecret(), pubkey);

        Send(encResp);

        delete m_Encrypter;
        m_Encrypter = aesEncrypter;
    }

    void HandlePacket(Minecraft::Packets::Inbound::LoginSuccessPacket* packet) {
        std::cout << "Successfully logged in. Username: ";
        std::wcout << packet->GetUsername() << std::endl;

        m_ProtocolState = Minecraft::ProtocolState::Play;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetCompressionPacket* packet) {
        delete m_Compressor;
        m_Compressor = new Minecraft::CompressionZ();
    }

    void HandlePacket(Minecraft::Packets::Inbound::JoinGamePacket* packet) {
        std::cout << "Joining game with entity id of " << packet->GetEntityId() << std::endl;
        std::cout << "Game difficulty: " << (int)packet->GetDifficulty() << ", Dimension: " << (int)packet->GetDimension() << std::endl;
        std::wcout << "Level type: " << packet->GetLevelType() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet) {
        std::wcout << "Plugin message received on channel " << packet->GetChannel() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ServerDifficultyPacket* packet) {
        std::wcout << "New server difficulty: " << (int)packet->GetDifficulty() << std::endl;
    }

    bool Connect() {
        auto addrs = Network::Dns::Resolve(m_Server);

        if (addrs.size() == 0) return false;
        
        for (auto addr : addrs) {
            if (m_Socket->Connect(addr, m_Port))
                return true;            
        }

        return false;
    }

    Minecraft::Packets::Packet* CreatePacket(Minecraft::DataBuffer& buffer) {
        std::size_t readOffset = buffer.GetReadOffset();
        Minecraft::VarInt length;
        buffer >> length;

        if (buffer.GetRemaining() < (u32)length.GetInt()) {
            buffer.SetReadOffset(readOffset);
            return nullptr;
        }

        Minecraft::DataBuffer decompressed = m_Compressor->Decompress(buffer, length.GetInt());
        
        return Minecraft::Packets::PacketFactory::CreatePacket(m_ProtocolState, decompressed, length.GetInt());
    }

    void Run() {
        if (!Connect()) {
            std::cerr << "Couldn't connect\n";
            return;
        }

        Minecraft::Packets::Outbound::HandshakePacket handshake(47, m_Server, m_Port, Minecraft::ProtocolState::Login);
        Send(&handshake);

        Minecraft::Packets::Outbound::LoginStartPacket loginStart(PlayerName);
        Send(&loginStart);

        m_ProtocolState = Minecraft::ProtocolState::Login;

        Minecraft::DataBuffer toHandle;

        while (true) {
            Minecraft::DataBuffer buffer = m_Socket->Receive(4096);

            if (m_Socket->GetStatus() != Network::Socket::Connected) break;

            toHandle << m_Encrypter->Decrypt(buffer);

            Minecraft::Packets::Packet* packet = CreatePacket(toHandle);

            while (packet) {
                m_Dispatcher.Dispatch(packet);

                if (toHandle.IsFinished()) {
                    toHandle = Minecraft::DataBuffer();
                    break;
                }

                toHandle = Minecraft::DataBuffer(toHandle, toHandle.GetReadOffset());

                packet = CreatePacket(toHandle);
            }
        }
    }

    void Send(Minecraft::Packets::Packet* packet) {
        Minecraft::DataBuffer packetBuffer = packet->Serialize();
        Minecraft::DataBuffer buffer;
        Minecraft::VarInt packetSize = (s32)packetBuffer.GetSize();
        buffer << packetSize;
        buffer << packetBuffer;

        // todo: compression

        m_Socket->Send(m_Encrypter->Encrypt(buffer));
    }

};


#include "Entity.h"

int main(void) {
    Connection conn("192.168.2.5", 25565);
    
    conn.Run();

    return 0;
}
