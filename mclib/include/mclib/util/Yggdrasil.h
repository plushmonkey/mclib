#ifndef MCLIB_UTIL_YGGDRASIL_H_
#define MCLIB_UTIL_YGGDRASIL_H_

#include <mclib/common/JsonFwd.h>
#include <mclib/common/Types.h>
#include <mclib/common/UUID.h>
#include <mclib/util/HTTPClient.h>

#include <exception>
#include <memory>
#include <string>

namespace mc {
namespace util {

// Todo: maybe add different exceptions for the different server responses to
// make error handling easier.

class YggdrasilException : public std::exception {
private:
    std::string m_ErrorMessage;

public:
    YggdrasilException(const std::string& message) : m_ErrorMessage(message) {}

    YggdrasilException(const std::string& error,
                       const std::string& errorMessage)
        : m_ErrorMessage(error + ": " + errorMessage) {}

    virtual const char* what() const NOEXCEPT { return m_ErrorMessage.c_str(); }
};

class Yggdrasil {
private:
    std::unique_ptr<HTTPClient> m_Http;
    std::string m_PlayerName;
    std::string m_AuthUrl;
    std::string m_SessionUrl;
    std::string m_AccessToken;
    std::string m_ClientToken;
    std::string m_ProfileId;

    void Initialize();

public:
    Yggdrasil() : m_Http(std::make_unique<CurlHTTPClient>()) { Initialize(); }

    Yggdrasil(std::unique_ptr<HTTPClient> http) : m_Http(std::move(http)) {
        Initialize();
    }

    MCLIB_API Yggdrasil(const Yggdrasil& other);
    MCLIB_API Yggdrasil& operator=(const Yggdrasil& other);

    const std::string& GetAccessToken() const { return m_AccessToken; }
    const std::string& GetClientToken() const { return m_ClientToken; }
    const std::string& GetPlayerName() const { return m_PlayerName; }
    const std::string& GetProfileId() const { return m_ProfileId; }

    void SetProfileId(const std::string& profileId) { m_ProfileId = profileId; }

    bool IsAuthenticated() const { return !m_AccessToken.empty(); }

    /**
     * Receives an access token from the auth server.
     * @param username The player's account login (username for old accounts,
     * email for migrated).
     * @param password The password for that player's account.
     * @param client A unique client token. Must be the same for every request.
     * Uses a default client if none is provided.
     * @return true if the login was successful.
     */
    bool MCLIB_API Authenticate(const std::string& username,
                                const std::string& password,
                                const std::string& client = "");

    /**
     * Posts a new session to the session server.
     * Computes the sha1 hex digest then posts it to the api.
     * @param serverId The id of the server received in EncryptionRequestPacket.
     * @param sharedSecret The shared secret that was generated when encryption
     * started.
     * @param publicKey The public key received in EncryptionRequestPacket.
     * @return true if it successfully posted the session to the server.
     * @throws YggdrasilException if it can't connect to the server, or if it
     * receives an error from the server.
     */
    bool MCLIB_API JoinServer(const std::wstring& serverId,
                              const std::string& sharedSecret,
                              const std::string& publicKey);

    /**
     * Posts a new session to the session server.
     * The server hash is a sha1 hex digest.
     * First it's updated with the serverID string,
     * then it's updated with the shared secret,
     * finally it's updated with public key.
     * @param serverHash the java-style hex digest
     * @throws YggdrasilException if it can't connect to the server, or if it
     * receives an error from the server.
     */
    bool MCLIB_API JoinServer(const std::string& serverHash);

    /**
     * Refreshes an access token by creating a new one and invalidating the old
     * one. Passwords should never be stored on file, so this is used to keep a
     * stored access token valid. If successful, the new accessToken will be
     * associated with the same clientToken as before.
     * @param accessToken The access token to refresh.
     * @param clientToken This should match the clientToken used to obtain the
     * accessToken originally.
     * @return A pair containing the access token and the username.
     */
    std::pair<std::string, std::string> MCLIB_API
    Refresh(const std::string& accessToken, const std::string& clientToken = 0);

    /**
     * Checks if an access token is usable for authentication. The token should
     * be refreshed if it isn't valid.
     * @param accessToken The access token to validate.
     * @param clientToken This should match the clientToken used to obtain the
     * accessToken originally.
     * @return True if the token is usable for authentication, false otherwise.
     */
    bool MCLIB_API Validate(const std::string& accessToken,
                            const std::string& clientToken = 0);

    /**
     * Invalidates the last used accessToken for an account using login
     * credentials. Use Invalidate to do it with the accessToken.
     * @param username Username for the account.
     * @param password Password for the account.
     */
    void MCLIB_API Signout(const std::string& username,
                           const std::string& password);

    /**
     * Invalidates the last used accessToken for an account using login
     * credentials. Use Invalidate to do it with the accessToken.
     * @param accessToken The access token to invalidate.
     * @param clientToken This should match the clientToken used to obtain the
     * accessToken originally.
     */
    void MCLIB_API Invalidate(const std::string& accessToken,
                              const std::string& clientToken = 0);

    UUID MCLIB_API GetPlayerUUID(const std::string& name);
    json MCLIB_API GetPlayerProfile(UUID& uuid);
};

}  // namespace util
}  // namespace mc

#endif
