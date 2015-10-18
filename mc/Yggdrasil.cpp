#include "Yggdrasil.h"

#include <iostream>
#include <json/json.h>

namespace {
const std::string DefaultClientToken = "pmToken";
}

namespace Minecraft {

void Yggdrasil::Initialize() {
    m_AuthUrl = "https://authserver.mojang.com/";
    m_SessionURL = "https://sessionserver.mojang.com/session/minecraft/";
}

JoinResponse Yggdrasil::JoinServer(const std::string& serverHash) {
    if (m_AccessToken.length() == 0)
        throw YggdrasilException("Error trying to join a server before authenticating.");

    Json::Value data;
    data["accessToken"] = m_AccessToken;
    data["selectedProfile"] = m_ProfileId;
    data["serverId"] = serverHash;

    HTTPResponse resp = m_Http->PostJSON(m_SessionURL + "join", data);
    if (resp.status == 0)
        throw YggdrasilException("Error connecting to session server.");

    return JoinResponse();

    /*Json::Reader reader;
    Json::Value result;
        
    if (!reader.parse(resp.body, result))
        throw YggdrasilException("Error parsing response from session server.");

    if (resp.status != 200) {
        if (!result["error"].isNull())
            throw YggdrasilException(result["error"].asString(), result["errorMessage"].asString());
        else
            throw YggdrasilException("Unknown error with session server. Status: " + std::to_string(resp.status) + ".");
    }

    JoinResponse joinResponse;

    joinResponse.profileId = result["id"].asString();
    joinResponse.playerName = result["name"].asString();
    joinResponse.textureValue = result["properties"][0]["value"].asString(); // base64 encoded
    joinResponse.signature = result["properties"][0]["signature"].asString();


    return joinResponse;*/
}

bool Yggdrasil::Authenticate(const std::string& username, const std::string& password, const std::string& client) {
    Json::Value authPayload;

    authPayload["agent"]["name"] = "Minecraft";
    authPayload["agent"]["version"] = 1;
    authPayload["username"] = username;
    authPayload["password"] = password;
    authPayload["clientToken"] = client.length() > 0 ? client : DefaultClientToken;

    HTTPResponse resp = m_Http->PostJSON(m_AuthUrl + "authenticate", authPayload);

    if (resp.status == 0) return false;

    Json::Reader reader;
    Json::Value result;

    if (!reader.parse(resp.body, result)) return false;
    
    if (!result["error"].isNull())
        throw YggdrasilException(result["error"].asString(), result["errorMessage"].asString());

    if (result["accessToken"].isNull())
        return false;

    m_AccessToken = result["accessToken"].asString();

    if (!result["clientToken"].isNull())
        m_ClientToken = result["clientToken"].asString();
    else
        m_ClientToken = authPayload["clientToken"].asString();

    if (result["selectedProfile"].isNull())
        throw YggdrasilException("No minecraft license attached to Mojang account.");

    m_ProfileId = result["selectedProfile"]["id"].asString();
    m_PlayerName = result["selectedProfile"]["name"].asString();

    return true;
}

} // ns Minecraft
