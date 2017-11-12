#include "TextureGrabber.h"

#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/util/Hash.h>
#include <mclib/util/HTTPClient.h>

#include <iostream>
#include <fstream>
#include <string>

namespace example {

bool TextureGrabber::ContainsTextureURL(const Json::Value& root) {
    if (!root.isMember("textures")) return false;
    if (!root["textures"].isMember("SKIN")) return false;
    return root["textures"]["SKIN"].isMember("url");
}

TextureGrabber::TextureGrabber(mc::protocol::packets::PacketDispatcher* dispatcher)
    : mc::protocol::packets::PacketHandler(dispatcher)
{
    using namespace mc::protocol;

    dispatcher->RegisterHandler(State::Play, play::PlayerListItem, this);
}

TextureGrabber::~TextureGrabber() {
    GetDispatcher()->UnregisterHandler(this);
}

void TextureGrabber::HandlePacket(mc::protocol::packets::in::PlayerListItemPacket* packet) {
    using namespace mc::protocol::packets::in;

    PlayerListItemPacket::Action action = packet->GetAction();

    if (action == PlayerListItemPacket::Action::AddPlayer) {
        auto actionDataList = packet->GetActionData();

        for (auto actionData : actionDataList) {
            auto properties = actionData->properties;

            auto iter = properties.find(L"textures");

            if (iter == properties.end()) continue;

            std::wstring encoded = iter->second;
            std::string decoded = mc::util::Base64Decode(std::string(encoded.begin(), encoded.end()));

            Json::Value root;
            Json::Reader reader;

            std::wstring name = actionData->name;

            if (!reader.parse(decoded, root)) {
                std::wcerr << L"Failed to parse decoded data for " << name;
                continue;
            }

            if (!ContainsTextureURL(root)) {
                std::wcerr << L"No texture found for " << name;
                continue;
            }

            std::string url = root["textures"]["SKIN"]["url"].asString();

            std::wcout << L"Fetching skin for " << name << std::endl;

            mc::util::CurlHTTPClient http;

            mc::util::HTTPResponse resp = http.Get(url);

            if (resp.status == 200) {
                std::wcout << L"Saving texture for " << name << std::endl;

                std::string body = resp.body;

                std::string filename = std::string(name.begin(), name.end()) + ".png";
                std::ofstream out(filename, std::ios::out | std::ios::binary);

                out.write(body.c_str(), body.size());
            }
        }
    }
}

} // ns example
