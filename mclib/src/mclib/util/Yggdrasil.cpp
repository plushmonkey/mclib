#include <mclib/util/Yggdrasil.h>

#include <mclib/util/Hash.h>

#include <iostream>
#include <json/json.h>
#include <openssl/sha.h>

namespace {
const std::string DefaultClientToken = "pmToken";
}

namespace mc {
namespace util {

void Yggdrasil::Initialize() {
    m_AuthUrl = "https://authserver.mojang.com/";
    m_SessionUrl = "https://sessionserver.mojang.com/session/minecraft/";
}

Yggdrasil::Yggdrasil(const Yggdrasil& other)
    : m_Http(std::make_unique<CurlHTTPClient>()),
      m_PlayerName(other.m_PlayerName),
      m_AuthUrl(other.m_AuthUrl),
      m_SessionUrl(other.m_SessionUrl),
      m_AccessToken(other.m_AccessToken),
      m_ClientToken(other.m_ClientToken),
      m_ProfileId(other.m_ProfileId)
{

}

Yggdrasil& Yggdrasil::operator=(const Yggdrasil& other) {
    m_Http = std::make_unique<CurlHTTPClient>();
    m_PlayerName = other.m_PlayerName;
    m_AuthUrl = other.m_AuthUrl;
    m_SessionUrl = other.m_SessionUrl;
    m_AccessToken = other.m_AccessToken;
    m_ClientToken = other.m_ClientToken;
    m_ProfileId = other.m_ProfileId;
    return *this;
}

bool Yggdrasil::JoinServer(const std::wstring& serverId, const std::string& sharedSecret, const std::string& publicKey) {
    SHA_CTX shaCtx;
    SHA1_Init(&shaCtx);

    std::string serverId8(serverId.begin(), serverId.end());
    SHA1_Update(&shaCtx, serverId8.c_str(), serverId8.size());
    SHA1_Update(&shaCtx, sharedSecret.c_str(), sharedSecret.length());
    SHA1_Update(&shaCtx, publicKey.c_str(), publicKey.length());

    unsigned char digest[20] = { 0 };
    SHA1_Final(digest, &shaCtx);

    std::string serverHash = Sha1HexDigest(digest);

    return this->JoinServer(serverHash);
}

bool Yggdrasil::JoinServer(const std::string& serverHash) {
    if (m_AccessToken.length() == 0)
        throw YggdrasilException("Error trying to join a server before authenticating.");

    Json::Value data;
    data["accessToken"] = m_AccessToken;
    data["selectedProfile"] = m_ProfileId;
    data["serverId"] = serverHash;

    HTTPResponse resp = m_Http->PostJSON(m_SessionUrl + "join", data);

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

    m_Http = std::make_unique<CurlHTTPClient>();

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

std::pair<std::string, std::string> Yggdrasil::Refresh(const std::string& accessToken, const std::string& clientToken) {
    Json::Value payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] = clientToken.length() > 0 ? clientToken : DefaultClientToken;

    HTTPResponse resp = m_Http->PostJSON(m_AuthUrl + "refresh", payload);

    if (resp.status == 0)
        throw YggdrasilException("No response received while refreshing access token.");

    Json::Reader reader;
    Json::Value result;

    if (!reader.parse(resp.body, result))
        throw YggdrasilException("Could not parse JSON response while refreshing access token.");

    if (!result["error"].isNull())
        throw YggdrasilException(result["error"].asString(), result["errorMessage"].asString());

    m_AccessToken = result["accessToken"].asString();
    m_ClientToken = clientToken;

    if (!result["selectedProfile"].isNull()) {
        if (!result["selectedProfile"]["id"].isNull())
            m_ProfileId = result["selectedProfile"]["id"].asString();

        if (!result["selectedProfile"]["name"].isNull())
            m_PlayerName = result["selectedProfile"]["name"].asString();
    }

    return std::make_pair(m_AccessToken, m_PlayerName);
}

bool Yggdrasil::Validate(const std::string& accessToken, const std::string& clientToken) {
    Json::Value payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] = clientToken.length() > 0 ? clientToken : DefaultClientToken;

    HTTPResponse resp = m_Http->PostJSON(m_AuthUrl + "validate", payload);

    if (resp.status == 200 || resp.status == 204) {
        m_AccessToken = accessToken;
        m_ClientToken = clientToken;
        return true;
    }

    return false;
}

void Yggdrasil::Signout(const std::string& username, const std::string& password) {
    Json::Value payload;

    payload["username"] = username;
    payload["password"] = password;

    m_Http->PostJSON(m_AuthUrl + "signout", payload);
}

void Yggdrasil::Invalidate(const std::string& accessToken, const std::string& clientToken) {
    Json::Value payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] = clientToken.length() > 0 ? clientToken : DefaultClientToken;

    m_Http->PostJSON(m_AuthUrl + "invalidate", payload);
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

} // ns util
} // ns mc
