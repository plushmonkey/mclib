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

bool AuthToken::Validate() {
    if (m_AccessToken.empty() || m_ClientToken.empty())
        return false;

    // Only try to validate the token if the profile id is stored.
    if (!m_ProfileId.empty() && m_Yggdrasil->Validate(m_AccessToken, m_ClientToken)) {
        m_Valid = true;
        m_Yggdrasil->SetProfileId(m_ProfileId);
        return true;
    }

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
