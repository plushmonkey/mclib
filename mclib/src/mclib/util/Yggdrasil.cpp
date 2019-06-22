#include <mclib/util/Yggdrasil.h>

#include <mclib/common/Json.h>
#include <mclib/util/Hash.h>

#include <openssl/sha.h>
#include <iostream>

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
      m_ProfileId(other.m_ProfileId) {}

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

bool Yggdrasil::JoinServer(const std::wstring& serverId,
                           const std::string& sharedSecret,
                           const std::string& publicKey) {
    SHA_CTX shaCtx;
    SHA1_Init(&shaCtx);

    std::string serverId8(serverId.begin(), serverId.end());
    SHA1_Update(&shaCtx, serverId8.c_str(), serverId8.size());
    SHA1_Update(&shaCtx, sharedSecret.c_str(), sharedSecret.length());
    SHA1_Update(&shaCtx, publicKey.c_str(), publicKey.length());

    unsigned char digest[20] = {0};
    SHA1_Final(digest, &shaCtx);

    std::string serverHash = Sha1HexDigest(digest);

    return this->JoinServer(serverHash);
}

bool Yggdrasil::JoinServer(const std::string& serverHash) {
    if (m_AccessToken.length() == 0)
        throw YggdrasilException(
            "Error trying to join a server before authenticating.");

    json data;

    data["accessToken"] = m_AccessToken;
    data["selectedProfile"] = m_ProfileId;
    data["serverId"] = serverHash;

    HTTPResponse resp = m_Http->PostJSON(m_SessionUrl + "join", data);

    // Always returns 204 No Content, but it might change in the future
    return resp.status >= 200 && resp.status < 300;
}

bool Yggdrasil::Authenticate(const std::string& username,
                             const std::string& password,
                             const std::string& client) {
    json authPayload;

    authPayload["agent"]["name"] = "Minecraft";
    authPayload["agent"]["version"] = 1;
    authPayload["username"] = username;
    authPayload["password"] = password;
    authPayload["clientToken"] =
        client.length() > 0 ? client : DefaultClientToken;

    m_Http = std::make_unique<CurlHTTPClient>();

    HTTPResponse resp =
        m_Http->PostJSON(m_AuthUrl + "authenticate", authPayload);

    if (resp.status == 0) return false;

    json result;

    try {
        result = json::parse(resp.body);
    } catch (json::parse_error&) {
        return false;
    }

    auto errorNode = result.value("error", json());
    if (!errorNode.is_null())
        throw YggdrasilException(errorNode.get<std::string>(),
                                 result["errorMessage"].get<std::string>());

    auto accessTokenNode = result.value("accessToken", json());
    if (accessTokenNode.is_null()) return false;

    m_AccessToken = accessTokenNode.get<std::string>();

    auto clientTokenNode = result.value("clientToken", json());

    if (!clientTokenNode.is_null())
        m_ClientToken = clientTokenNode.get<std::string>();
    else
        m_ClientToken = authPayload["clientToken"].get<std::string>();

    auto selectedProfileNode = result.value("selectedProfile", json());

    if (selectedProfileNode.is_null())
        throw YggdrasilException(
            "No minecraft license attached to Mojang account.");

    m_ProfileId = selectedProfileNode["id"].get<std::string>();
    m_PlayerName = selectedProfileNode["name"].get<std::string>();

    return true;
}

std::pair<std::string, std::string> Yggdrasil::Refresh(
    const std::string& accessToken, const std::string& clientToken) {
    json payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] =
        clientToken.length() > 0 ? clientToken : DefaultClientToken;

    HTTPResponse resp = m_Http->PostJSON(m_AuthUrl + "refresh", payload);

    if (resp.status == 0)
        throw YggdrasilException(
            "No response received while refreshing access token.");

    json result;

    try {
        result = json::parse(resp.body);
    } catch (json::parse_error&) {
        throw YggdrasilException(
            "Could not parse JSON response while refreshing access token.");
    }

    auto errorNode = result.value("error", json());

    if (!errorNode.is_null())
        throw YggdrasilException(errorNode.get<std::string>(),
                                 result["errorMessage"].get<std::string>());

    m_AccessToken = result["accessToken"].get<std::string>();
    m_ClientToken = clientToken;

    auto selectedProfileNode = result.value("selectedProfile", json());

    if (!selectedProfileNode.is_null()) {
        if (selectedProfileNode.find("id") != selectedProfileNode.end()) {
            m_ProfileId = selectedProfileNode["id"].get<std::string>();
        }

        if (selectedProfileNode.find("name") != selectedProfileNode.end()) {
            m_PlayerName = selectedProfileNode["name"].get<std::string>();
        }
    }

    return std::make_pair(m_AccessToken, m_PlayerName);
}

bool Yggdrasil::Validate(const std::string& accessToken,
                         const std::string& clientToken) {
    json payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] =
        clientToken.length() > 0 ? clientToken : DefaultClientToken;

    HTTPResponse resp = m_Http->PostJSON(m_AuthUrl + "validate", payload);

    if (resp.status == 200 || resp.status == 204) {
        m_AccessToken = accessToken;
        m_ClientToken = clientToken;
        return true;
    }

    return false;
}

void Yggdrasil::Signout(const std::string& username,
                        const std::string& password) {
    json payload;

    payload["username"] = username;
    payload["password"] = password;

    m_Http->PostJSON(m_AuthUrl + "signout", payload);
}

void Yggdrasil::Invalidate(const std::string& accessToken,
                           const std::string& clientToken) {
    json payload;

    payload["accessToken"] = accessToken;
    payload["clientToken"] =
        clientToken.length() > 0 ? clientToken : DefaultClientToken;

    m_Http->PostJSON(m_AuthUrl + "invalidate", payload);
}

UUID Yggdrasil::GetPlayerUUID(const std::string& name) {
    std::string url = "https://api.mojang.com/users/profiles/minecraft/" + name;

    HTTPResponse resp =
        m_Http->Get(url, {{"Content-Type", "application/json"}});

    if (resp.status == 0) throw YggdrasilException("No response from server.");

    json result;

    try {
        result = json::parse(resp.body);
    } catch (json::parse_error&) {
        throw YggdrasilException("Failed to parse data received from server.");
    }

    if (result.find("error") != result.end())
        throw YggdrasilException(result["error"].get<std::string>(),
                                 result["errorMessage"].get<std::string>());

    std::string uuidStr = result["id"].get<std::string>();

    return UUID::FromString(uuidStr, false);
}

json Yggdrasil::GetPlayerProfile(UUID& uuid) {
    std::string url =
        "https://sessionserver.mojang.com/session/minecraft/profile/" +
        uuid.ToString(false);

    HTTPResponse resp =
        m_Http->Get(url, {{"Content-Type", "application/json"}});

    if (resp.status == 0) throw YggdrasilException("No response from server.");

    json result;

    try {
        result = json::parse(resp.body);
    } catch (json::parse_error&) {
        throw YggdrasilException("Failed to parse data received from server.");
    }

    if (result.find("error") != result.end())
        throw YggdrasilException(result["error"].get<std::string>(),
                                 result["errorMessage"].get<std::string>());

    return result;
}

}  // namespace util
}  // namespace mc
