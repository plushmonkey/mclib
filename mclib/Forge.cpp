#include "Forge.h"
#include "Packets/PacketDispatcher.h"

#include <map>
#include <functional>
#include "Utility.h"
#include "DataBuffer.h"

using namespace Minecraft::Forge;

struct RegisterPacket {
    std::vector<std::string> channels;

    Minecraft::DataBuffer Serialize() {
        Minecraft::DataBuffer buffer;

        u8 nullChar = '\0';

        for (std::string channel : channels) {
            buffer << channel << nullChar;
        }

        return buffer;
    }
};

enum class ForgePacket {
    ServerHello = 0,
    ClientHello,
    ModList,
    RegistryData,

    HandshakeAck = 255,
    HandshakeReset = 254
};

enum class HandshakeClientPhase {
    Start,
    Hello,
    WaitingServerData,
    WaitingServerComplete,
    PendingComplete,
    Complete,
    Done,
    Error
};

enum class HandshakeServerPhase {
    Start,
    Hello,
    WaitingAck,
    Complete,
    Done,
    Error
};

struct ServerHelloPacket {
    u8 discriminator;
    u8 version;
    s32 dimension;

    void Deserialize(Minecraft::DataBuffer buffer) {
        buffer >> discriminator >> version;
        if (version > 1 && buffer.GetRemaining() > 0)
            buffer >> dimension;
    }
};

struct ClientHelloPacket {
    u8 discriminator;
    u8 version;

    ClientHelloPacket(u8 version) : discriminator(1), version(version) { }

    Minecraft::DataBuffer Serialize() {
        Minecraft::DataBuffer buffer;

        buffer << discriminator << version;

        return buffer;
    }
};

struct ModListPacket {
    u8 discriminator;
    std::vector<ForgeHandler::ModInfo> mods;

    ModListPacket(std::vector<ForgeHandler::ModInfo> mods) : discriminator(2), mods(mods) { }

    Minecraft::DataBuffer Serialize() {
        Minecraft::DataBuffer buffer;

        Minecraft::VarInt count((s32)mods.size());

        buffer << discriminator << count;

        for (ForgeHandler::ModInfo modInfo : mods) {
            Minecraft::MCString name(modInfo.name);
            Minecraft::MCString version(modInfo.version);

            buffer << name << version;
        }

        return buffer;
    }
};

struct RegistryDataPacket {
    u8 discriminator;
    bool hasMore;
    
    void Deserialize(Minecraft::DataBuffer buffer) {
        buffer >> discriminator >> hasMore;
    }
};

struct HandshakeAckPacket {
    u8 discriminator;
    u8 phase;

    HandshakeAckPacket(HandshakeClientPhase phase) : discriminator((u8)ForgePacket::HandshakeAck), phase((u8)phase) { }

    HandshakeAckPacket() { }

    Minecraft::DataBuffer Serialize() {
        Minecraft::DataBuffer buffer;

        buffer << discriminator << phase;

        return buffer;
    }

    void Deserialize(Minecraft::DataBuffer buffer) {
        buffer >> discriminator >> phase;
    }

    HandshakeServerPhase GetPhase() { return (HandshakeServerPhase)phase; }
};

ForgeHandler::ForgeHandler(Minecraft::Packets::PacketDispatcher* dispatcher, Minecraft::Connection* connection)
    : Minecraft::Packets::PacketHandler(dispatcher), m_Connection(connection), m_Version(2), m_ModInfoReceived(false)
{
    dispatcher->RegisterHandler(Minecraft::Protocol::State::Play, Minecraft::Protocol::Play::PluginMessage, this);
    dispatcher->RegisterHandler(Minecraft::Protocol::State::Status, Minecraft::Protocol::Status::Response, this);

    m_Handlers = HandlerMap {
        { L"FML|HS", std::bind(&ForgeHandler::HandleData, this, std::placeholders::_1) },
    };
}

ForgeHandler::~ForgeHandler() {
    GetDispatcher()->UnregisterHandler(this);
}

bool ForgeHandler::HasModInfo() const {
    return m_ModInfoReceived;
}

void ForgeHandler::HandlePacket(Minecraft::Packets::Inbound::Status::ResponsePacket* packet) {
    std::string response = Minecraft::to_string(packet->GetResponse());

    Json::Value data;
    Json::Reader reader;

    if (!reader.parse(response, data)) {
        m_ModInfoReceived = true;
        return;
    }

    Json::Value modInfo = data["modinfo"];
    if (modInfo.isNull()) {
        m_ModInfoReceived = true;
        return;
    }

    Json::Value modList = modInfo["modList"];
    if (!modList.isArray()) {
        m_ModInfoReceived = true;
        return;
    }

    for (auto iter = modList.begin(); iter != modList.end(); ++iter) {
        Json::Value modObject = (*iter);
        if (modObject.isNull()) continue;

        if (!modObject["modid"].isString() || !modObject["version"].isString()) continue;

        std::string id = modObject["modid"].asString();
        std::string ver = modObject["version"].asString();

        m_Mods.emplace_back(id, ver);
    }

    m_ModInfoReceived = true;
}

void ForgeHandler::HandlePacket(Minecraft::Packets::Inbound::PluginMessagePacket* packet) {
    std::wstring channel = packet->GetChannel();

    auto iter = m_Handlers.find(channel);
    if (iter == m_Handlers.end()) {
        return;
    }

    iter->second(packet->GetData());
}

void ForgeHandler::HandleData(const std::string& data) {
    if (data.empty()) return;

    ForgePacket discriminator = (ForgePacket)data[0];

    switch (discriminator) {
        case ForgePacket::ServerHello:
        {
            ServerHelloPacket serverHello;
            serverHello.Deserialize(Minecraft::DataBuffer(data));

            RegisterPacket registry;
            registry.channels.push_back("FML|HS");
            registry.channels.push_back("FML");
            registry.channels.push_back("FML|MP");
            registry.channels.push_back("FML");
            registry.channels.push_back("FORGE");
            SendPluginMessage(L"REGISTER", registry);

            ClientHelloPacket response(serverHello.version);
            SendPluginMessage(L"FML|HS", response);

            ModListPacket modListPacket(m_Mods);
            SendPluginMessage(L"FML|HS", modListPacket);
        }
        break;
        case ForgePacket::ModList:
        {
            HandshakeAckPacket ack(HandshakeClientPhase::WaitingServerData);
            SendPluginMessage(L"FML|HS", ack);
        }
        break;
        case ForgePacket::RegistryData:
        {
            RegistryDataPacket registryData;
            registryData.Deserialize(Minecraft::DataBuffer(data));

            if (!registryData.hasMore) {
                HandshakeAckPacket ack(HandshakeClientPhase::WaitingServerComplete);
                SendPluginMessage(L"FML|HS", ack);
            }
        }
        break;
        case ForgePacket::HandshakeAck:
        {
            HandshakeAckPacket ack;
            ack.Deserialize(Minecraft::DataBuffer(data));

            HandshakeServerPhase phase = ack.GetPhase();
            switch (phase) {
                case HandshakeServerPhase::WaitingAck:
                {
                    HandshakeAckPacket ack(HandshakeClientPhase::PendingComplete);
                    SendPluginMessage(L"FML|HS", ack);
                }
                break;
                case HandshakeServerPhase::Complete:
                {
                    HandshakeAckPacket ack(HandshakeClientPhase::Complete);
                    SendPluginMessage(L"FML|HS", ack);
                }
                break;
                default:
                break;
            }
        }
        break;
        default:
        break;
    }
}
