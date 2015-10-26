#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <array>
#include <vector>
#include <json/json.h>
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
#include <regex>

/** This is all junk code just to get things working */

#include <windows.h> // timeGetTime

#pragma comment(lib, "winmm.lib")

// TODO: Switch to using vectors in packets instead of individual variables
// TODO: Create player manager
// TODO: Create entity system and entity manager
// TODO: Create world structure from chunk columns

std::string PlayerName = "testplayer";
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
    Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* m_LastPosition;

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

        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x00, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x01, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x02, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x05, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x06, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x08, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x0C, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x0F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x15, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x1C, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x1F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x20, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x26, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x2F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x30, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x37, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x38, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x39, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x3F, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x41, this);
        m_Dispatcher.RegisterHandler(Minecraft::ProtocolState::Play, 0x44, this);

        m_LastPosition = nullptr;
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityPropertiesPacket* packet) {
        std::cout << "Received entity properties: " << std::endl;
        const auto& properties = packet->GetProperties();
        for (const auto& kv : properties) {
            std::wstring key = kv.first;
            const auto& property = kv.second;
            std::wcout << key << " : " << property.value << std::endl;
            for (const auto& modifier : property.modifiers) 
                std::cout << "Modifier: " << modifier.uuid << " " << modifier.amount << " " << (int)modifier.operation << std::endl;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityRelativeMovePacket* packet) {
        if (packet->GetDeltaX() != 0 || packet->GetDeltaY() != 0 || packet->GetDeltaZ() != 0) {
            std::cout << "Entity " << packet->GetEntityId() << " relative move: (";
            std::cout << packet->GetDeltaX() << ", " << packet->GetDeltaY() << ", " << packet->GetDeltaZ() << ")" << std::endl;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnPlayerPacket* packet) {
        float x, y, z;
        x = packet->GetX();
        y = packet->GetY();
        z = packet->GetZ();
        std::cout << "Spawn player " << packet->GetUUID() << " at (" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::UpdateHealthPacket* packet) {
        std::cout << "Set health. health: " << packet->GetHealth() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetExperiencePacket* packet) {
        std::cout << "Set experience. Level: " << packet->GetLevel() << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::ChatPacket* packet) {
        const Json::Value& root = packet->GetChatData();

        Json::StyledWriter writer;
        std::string out = writer.write(root);
        std::cout << out << std::endl;

        return;
        if (!root["text"].isString()) return;

        std::string text = root["text"].asString();
        if (!root["extra"].isNull()) {
            for (Json::ValueConstIterator iter = root["extra"].begin(); iter != root["extra"].end(); ++iter) {
                if (!iter->isString()) continue;

                text += iter->asString();
            }
        }

        std::regex regex(R"::(<(.*?)> (.+)$)::");

        std::sregex_iterator iter(text.begin(), text.end(), regex);
        std::sregex_iterator end;
        if (iter != end) {
            std::smatch match = *iter;

            std::string sender = match[1];
            std::string message = match[2];

            if (sender.compare(PlayerName) == 0)
                return;

            Minecraft::Packets::Outbound::ChatPacket chatResponse(message);
            Send(&chatResponse);
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::EntityMetadataPacket* packet) {
        const auto& metadata = packet->GetMetadata();

        //std::cout << "Received entity metadata" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::SpawnMobPacket* packet) {
        const auto& metadata = packet->GetMetadata();
        
        //std::cout << "Received SpawnMobPacket" << std::endl;
    }

    void HandlePacket(Minecraft::Packets::Inbound::MapChunkBulkPacket* packet) {
        std::cout << "Received MapChunkBulkPacket" << std::endl;

        s32 x = (s32)m_LastPosition->GetX();
        s32 y = (s32)m_LastPosition->GetY();
        s32 z = (s32)m_LastPosition->GetZ();

        std::pair<s32, s32> key(x / 16, z / 16);

        auto& chunkMap = packet->GetChunkColumns();
        if (chunkMap.find(key) != chunkMap.end()) {
            Minecraft::BlockPtr block = chunkMap.at(key)->GetBlock(Vector3i(x % 16, y - 1, z % 16));

            if (block) {
                s16 type = block->type;

                std::cout << "Standing on block type " << type << std::endl;
            } else {
                std::cout << "Couldn't find block" << std::endl;
            }
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::SetSlotPacket* packet) {
        Minecraft::Slot slot = packet->GetSlot();
        int window = packet->GetWindowId();
        int index = packet->GetSlotIndex();

        std::cout << "Set slot (" << window << ", " << index << ") = " << slot.GetItemId() << ":" << slot.GetItemDamage() << "\n";
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
                std::cout << "World border warning time: " << packet->GetWarningTime() << " seconds, blocks: " << packet->GetWarningBlocks() << " meters" << std::endl;
            }
            break;
        }
    }

    void HandlePacket(Minecraft::Packets::Inbound::KeepAlivePacket* packet) {
        Minecraft::Packets::Outbound::KeepAlivePacket response(packet->GetAliveId());
        Send(&response);
    }

    void HandlePacket(Minecraft::Packets::Inbound::PlayerPositionAndLookPacket* packet) {
        s32 x = (s32)packet->GetX();
        s32 y = (s32)packet->GetY(); 
        s32 z = (s32)packet->GetZ();
        Minecraft::Position pos(x, y , z);

        std::cout << "Pos: " << pos << std::endl;

        // I guess the client has to figure this out from chunk data?
        bool onGround = true;

        using namespace Minecraft::Packets;

        if (m_LastPosition)
            delete m_LastPosition;

        // Used to verify position
        Outbound::PlayerPositionAndLookPacket response(packet->GetX(), packet->GetY(), packet->GetZ(),
            packet->GetYaw(), packet->GetPitch(), onGround);

        Send(&response);

        Outbound::ClientStatusPacket status(Outbound::ClientStatusPacket::Action::PerformRespawn);
        Send(&status);

        m_LastPosition = new Inbound::PlayerPositionAndLookPacket();
        *m_LastPosition = *packet;
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
        m_Compressor = new Minecraft::CompressionZ(packet->GetMaxPacketSize());
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

    void UpdatePosition() {
        if (!m_LastPosition) return;
        static DWORD lastSend = 0;
        
        if (timeGetTime() - lastSend >= 50) {
            bool onGround = true;

            Minecraft::Packets::Outbound::PlayerPositionAndLookPacket response(m_LastPosition->GetX(), m_LastPosition->GetY(), m_LastPosition->GetZ(),
                m_LastPosition->GetYaw(), m_LastPosition->GetPitch(), onGround);

            Send(&response);

            lastSend = timeGetTime();
        }
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

            UpdatePosition();

            toHandle << m_Encrypter->Decrypt(buffer);

            Minecraft::Packets::Packet* packet = nullptr;

            do {
                try {
                    packet = CreatePacket(toHandle);
                    if (packet) {
                        m_Dispatcher.Dispatch(packet);
                        Minecraft::Packets::PacketFactory::FreePacket(packet);
                    } else {
                        break;
                    }
                } catch (const std::exception& e) {
                    //std::cerr << e.what() << std::endl;
                    // Temporary until protocol is finished
                }
            } while (!toHandle.IsFinished() && toHandle.GetSize() > 0);

            if (toHandle.IsFinished())
                toHandle = Minecraft::DataBuffer();
            else if (toHandle.GetReadOffset() != 0)
                toHandle = Minecraft::DataBuffer(toHandle, toHandle.GetReadOffset());
        }
    }

    void Send(Minecraft::Packets::Packet* packet) {
        Minecraft::DataBuffer packetBuffer = packet->Serialize();
        Minecraft::DataBuffer buffer = m_Compressor->Compress(packetBuffer);

        m_Socket->Send(m_Encrypter->Encrypt(buffer));
    }

};

int main(void) {
    Connection conn("192.168.2.5", 25565);
    //Connection conn("play.mysticempire.net", 25565);
    
    conn.Run();

    return 0;
}
