#ifndef MCLIB_CORE_AUTHTOKEN_H_
#define MCLIB_CORE_AUTHTOKEN_H_

#include <mclib/mclib.h>
#include <memory>
#include <string>

namespace mc {

namespace util {

class Yggdrasil;

}  // namespace util

namespace core {

class AuthToken {
private:
    std::unique_ptr<util::Yggdrasil> m_Yggdrasil;
    std::string m_AccessToken;
    std::string m_ClientToken;
    std::string m_ProfileId;
    bool m_Valid;

public:
    MCLIB_API AuthToken();
    MCLIB_API AuthToken(const std::string& accessToken,
                        const std::string& clientToken,
                        const std::string& profileId = "");

    MCLIB_API AuthToken(const AuthToken& rhs);
    MCLIB_API AuthToken& operator=(const AuthToken& rhs);
    MCLIB_API AuthToken(AuthToken&& rhs) = default;
    MCLIB_API AuthToken& operator=(AuthToken&& rhs) = default;

    // Checks the api to see if the access token is valid.
    MCLIB_API bool Validate(const std::string& username = "");
    // This will invalidate the accessToken and get a new one if possible.
    MCLIB_API bool Refresh();

    inline bool IsValid() const noexcept { return m_Valid; }
    inline const std::string& GetAccessToken() const noexcept {
        return m_AccessToken;
    }
    inline const std::string& GetClientToken() const noexcept {
        return m_ClientToken;
    }
    std::unique_ptr<util::Yggdrasil>& GetYggdrasil() { return m_Yggdrasil; }
};

}  // namespace core
}  // namespace mc

#endif
