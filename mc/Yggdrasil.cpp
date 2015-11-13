#include "Yggdrasil.h"

#include <iostream>
#include <json/json.h>
#include <openssl/sha.h>
#include "Hash.h"

namespace {
const std::string DefaultClientToken = "pmToken";
}

namespace Minecraft {

void Yggdrasil::Initialize() {
    m_AuthUrl = "https://authserver.mojang.com/";
    m_SessionURL = "https://sessionserver.mojang.com/session/minecraft/";
}

bool Yggdrasil::JoinServer(const std::wstring& serverId, const std::string& sharedSecret, const std::string& publicKey) {
    SHA_CTX shaCtx;
    SHA1_Init(&shaCtx);
    SHA1_Update(&shaCtx, serverId.c_str(), serverId.size());
    SHA1_Update(&shaCtx, sharedSecret.c_str(), sharedSecret.length());
    SHA1_Update(&shaCtx, publicKey.c_str(), publicKey.length());

    unsigned char digest[20] = { 0 };
    SHA1_Final(digest, &shaCtx);

    std::string serverHash = Sha::Sha1HexDigest(digest);

    return this->JoinServer(serverHash);
}

bool Yggdrasil::JoinServer(const std::string& serverHash) {
    if (m_AccessToken.length() == 0)
        throw YggdrasilException("Error trying to join a server before authenticating.");

    Json::Value data;
    data["accessToken"] = m_AccessToken;
    data["selectedProfile"] = m_ProfileId;
    data["serverId"] = serverHash;

    HTTPResponse resp = m_Http->PostJSON(m_SessionURL + "join", data);

    // Always returns 204 No Content, but it might change in the future
    return resp.status >= 200 && resp.status < 300;
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

UUID Yggdrasil::GetPlayerUUID(const std::string& name) {
    std::string url = "https://api.mojang.com/users/profiles/minecraft/" + name;

    HTTPResponse resp = m_Http->Get(url, { { "Content-Type", "application/json" } });

    if (resp.status == 0) 
        throw YggdrasilException("No response from server.");

    Json::Reader reader;
    Json::Value result;

    if (!reader.parse(resp.body, result))
        throw YggdrasilException("Failed to parse data received from server.");

    if (!result["error"].isNull())
        throw YggdrasilException(result["error"].asString(), result["errorMessage"].asString());

    std::string uuidStr = result["id"].asString();

    return UUID::FromString(uuidStr, false);
}

Json::Value Yggdrasil::GetPlayerProfile(UUID& uuid) {
    std::string url = "https://sessionserver.mojang.com/session/minecraft/profile/" + uuid.ToString(false);

    HTTPResponse resp = m_Http->Get(url, { { "Content-Type", "application/json" } });

    if (resp.status == 0)
        throw YggdrasilException("No response from server.");

    Json::Reader reader;
    Json::Value result;

    if (!reader.parse(resp.body, result))
        throw YggdrasilException("Failed to parse data received from server.");

    if (!result["error"].isNull())
        throw YggdrasilException(result["error"].asString(), result["errorMessage"].asString());

    return result;
}

} // ns Minecraft
