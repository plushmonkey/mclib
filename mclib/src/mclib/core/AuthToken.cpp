#include <mclib/core/AuthToken.h>

#include <mclib/util/Yggdrasil.h>

namespace mc {
namespace core {

AuthToken::AuthToken() 
    : m_AccessToken(""),
      m_ClientToken(""),
      m_ProfileId(""),
      m_Valid(false),
      m_Yggdrasil(std::make_unique<util::Yggdrasil>())
{

}

AuthToken::AuthToken(const std::string& accessToken, const std::string& clientToken, const std::string& profileId)
    : m_AccessToken(accessToken),
      m_ClientToken(clientToken),
      m_ProfileId(profileId),
      m_Valid(false),
      m_Yggdrasil(std::make_unique<util::Yggdrasil>())
{

}

AuthToken::AuthToken(const AuthToken& rhs) 
    : m_AccessToken(rhs.m_AccessToken),
      m_ClientToken(rhs.m_ClientToken),
      m_ProfileId(rhs.m_ProfileId),
      m_Valid(rhs.m_Valid),
      m_Yggdrasil(std::make_unique<util::Yggdrasil>(*rhs.m_Yggdrasil))
{

}

AuthToken& AuthToken::operator=(const AuthToken& rhs) {
    m_AccessToken = rhs.m_AccessToken;
    m_ClientToken = rhs.m_ClientToken;
    m_ProfileId = rhs.m_ProfileId;
    m_Valid = rhs.m_Valid;
    m_Yggdrasil =std::make_unique<util::Yggdrasil>(*rhs.m_Yggdrasil);

    return *this;
}

bool AuthToken::Validate(const std::string& username) {
    if (m_AccessToken.empty() || m_ClientToken.empty())
        return false;

    if (m_ProfileId.empty() && !username.empty()) {
        auto http = std::make_unique<util::CurlHTTPClient>();
        auto result = http->Get("https://api.mojang.com/users/profiles/minecraft/" + username);
        if (result.status != 200) 
            return false;

        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(result.body, root))
            return false;

        if (root["id"].isString())
            m_ProfileId = root["id"].asString();
    }

    if (m_ProfileId.empty())
        return false;
    
    if (m_Yggdrasil->Validate(m_AccessToken, m_ClientToken)) {
        m_Valid = true;
        m_Yggdrasil->SetProfileId(m_ProfileId);
        return true;
    }

    return false;
}

bool AuthToken::Refresh() {
    try {
        auto pair = m_Yggdrasil->Refresh(m_AccessToken, m_ClientToken);
        m_AccessToken = pair.first;
        m_Valid = true;
    } catch (util::YggdrasilException& e) {
        return false;
    }

    return true;
}

} // ns core
} // ns mc
