#include <mclib/util/Forge.h>

#include <mclib/common/DataBuffer.h>
#include <mclib/common/Json.h>
#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/util/Utility.h>

#include <functional>
#include <map>

namespace mc {
namespace util {

struct RegisterPacket {
    std::vector<std::string> channels;

    DataBuffer Serialize() {
        DataBuffer buffer;

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

    void Deserialize(DataBuffer buffer) {
        buffer >> discriminator >> version;
        if (version > 1 && buffer.GetRemaining() > 0) buffer >> dimension;
    }
};

struct ClientHelloPacket {
    u8 discriminator;
    u8 version;

    ClientHelloPacket(u8 version) : discriminator(1), version(version) {}

    DataBuffer Serialize() {
        DataBuffer buffer;

        buffer << discriminator << version;

        return buffer;
    }
};

struct ModListPacket {
    u8 discriminator;
    std::vector<ForgeHandler::ModInfo> mods;

    ModListPacket(std::vector<ForgeHandler::ModInfo> mods)
        : discriminator(2), mods(mods) {}

    DataBuffer Serialize() {
        DataBuffer buffer;

        VarInt count((s32)mods.size());

        buffer << discriminator << count;

        for (ForgeHandler::ModInfo modInfo : mods) {
            MCString name(modInfo.name);
            MCString version(modInfo.version);

            buffer << name << version;
        }

        return buffer;
    }
};

struct RegistryDataPacket {
    u8 discriminator;
    bool hasMore;

    void Deserialize(DataBuffer buffer) { buffer >> discriminator >> hasMore; }
};

struct HandshakeAckPacket {
    u8 discriminator;
    u8 phase;

    HandshakeAckPacket(HandshakeClientPhase phase)
        : discriminator((u8)ForgePacket::HandshakeAck), phase((u8)phase) {}

    HandshakeAckPacket() {}

    DataBuffer Serialize() {
        DataBuffer buffer;

        buffer << discriminator << phase;

        return buffer;
    }

    void Deserialize(DataBuffer buffer) { buffer >> discriminator >> phase; }

    HandshakeServerPhase GetPhase() { return (HandshakeServerPhase)phase; }
};

ForgeHandler::ForgeHandler(protocol::packets::PacketDispatcher* dispatcher,
                           core::Connection* connection)
    : protocol::packets::PacketHandler(dispatcher),
      m_Connection(connection),
      m_Version(2),
      m_ModInfoReceived(false) {
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::PluginMessage, this);
    dispatcher->RegisterHandler(protocol::State::Status,
                                protocol::status::Response, this);

    m_Handlers = HandlerMap{
        {L"FML|HS",
         std::bind(&ForgeHandler::HandleData, this, std::placeholders::_1)},
    };
}

ForgeHandler::~ForgeHandler() { GetDispatcher()->UnregisterHandler(this); }

bool ForgeHandler::HasModInfo() const { return m_ModInfoReceived; }

void ForgeHandler::HandlePacket(
    protocol::packets::in::status::ResponsePacket* packet) {
    std::string response = mc::to_string(packet->GetResponse());

    json data;

    try {
        data = json::parse(response);
    } catch (json::parse_error&) {
        m_ModInfoReceived = true;
        return;
    }

    if (data.value("modinfo", json()).is_null()) {
        m_ModInfoReceived = true;
        return;
    }

    json modList = data["modinfo"].value("modList", json());
    if (!modList.is_array()) {
        m_ModInfoReceived = true;
        return;
    }

    for (auto iter = modList.begin(); iter != modList.end(); ++iter) {
        json modObject = (*iter);
        if (modObject.is_null()) continue;

        if (!modObject.value("modid", json()).is_string() ||
            !modObject.value("version", json()).is_string())
            continue;

        std::string id = modObject["modid"].get<std::string>();
        std::string ver = modObject["version"].get<std::string>();

        m_Mods.emplace_back(id, ver);
    }

    m_ModInfoReceived = true;
}

void ForgeHandler::HandlePacket(
    protocol::packets::in::PluginMessagePacket* packet) {
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
        case ForgePacket::ServerHello: {
            ServerHelloPacket serverHello;
            serverHello.Deserialize(DataBuffer(data));

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
        } break;
        case ForgePacket::ModList: {
            HandshakeAckPacket ack(HandshakeClientPhase::WaitingServerData);
            SendPluginMessage(L"FML|HS", ack);
        } break;
        case ForgePacket::RegistryData: {
            RegistryDataPacket registryData;
            registryData.Deserialize(DataBuffer(data));

            if (!registryData.hasMore) {
                HandshakeAckPacket ack(
                    HandshakeClientPhase::WaitingServerComplete);
                SendPluginMessage(L"FML|HS", ack);
            }
        } break;
        case ForgePacket::HandshakeAck: {
            HandshakeAckPacket ack;
            ack.Deserialize(DataBuffer(data));

            HandshakeServerPhase phase = ack.GetPhase();
            switch (phase) {
                case HandshakeServerPhase::WaitingAck: {
                    HandshakeAckPacket ack(
                        HandshakeClientPhase::PendingComplete);
                    SendPluginMessage(L"FML|HS", ack);
                } break;
                case HandshakeServerPhase::Complete: {
                    HandshakeAckPacket ack(HandshakeClientPhase::Complete);
                    SendPluginMessage(L"FML|HS", ack);
                } break;
                default:
                    break;
            }
        } break;
        default:
            break;
    }
}

}  // namespace util
}  // namespace mc
