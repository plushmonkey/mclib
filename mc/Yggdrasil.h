#ifndef YGGDRASIL_H_
#define YGGDRASIL_H_

#include "HTTPClient.h"
#include <string>
#include <exception>

namespace Minecraft {

// Todo: maybe add different exceptions for the different server responses to make error handling easier.

class YggdrasilException : public std::exception {
private:
    std::string m_ErrorMessage;

public:
    YggdrasilException(const std::string& message)
        : m_ErrorMessage(message)
    {
    }

    YggdrasilException(const std::string& error, const std::string& errorMessage) {
        m_ErrorMessage = error + ": " + errorMessage;
    }

    virtual const char* what() const {
        return m_ErrorMessage.c_str();
    }
};

struct JoinResponse {
    std::string profileId;
    std::string playerName;

    std::string textureValue;
    std::string signature;
};

class Yggdrasil {
private:
    HTTPClient* m_Http;
    std::string m_PlayerName;
    std::string m_AuthUrl;
    std::string m_SessionURL;
    std::string m_AccessToken;
    std::string m_ClientToken;
    std::string m_ProfileId;
    
    void Initialize();

    Yggdrasil(const Yggdrasil& other);
    Yggdrasil& operator=(const Yggdrasil& other);

public:
    Yggdrasil() 
        : m_Http(new CurlHTTPClient())
    {
        Initialize();
    }

    Yggdrasil(HTTPClient* http)
        : m_Http(http)
    {
        Initialize();
    }

    const std::string& GetAccessToken() const { return m_AccessToken; }
    const std::string& GetClientToken() const { return m_ClientToken; }
    const std::string& GetPlayerName() const { return m_PlayerName; }
    const std::string& GetProfileId() const { return m_ProfileId; }

    /**
     * Receives an access token from the auth server.
     * @param username The player's account login (username for old accounts, email for migrated).
     * @param password The password for that player's account.
     * @param client A unique client token. Must be the same for every request. Uses a default client if none is provided.
     * @return true if the login was successful.
     */
    bool Authenticate(const std::string& username, const std::string& password, const std::string& client = "");

    /**
     * Posts a new session to the session server.
     * The server hash is a sha1 hex digest.
     * First it's updated with the serverID string,
     * then it's updated with the shared secret,
     * finally it's updated with public key.
     * @param serverHash the java-style hex digest
     * @return a struct containing profileID, playerName, and texture data.
     * @throws YggdrasilException if it can't connect to the server, or if it receives an error from the server.
     */
    JoinResponse JoinServer(const std::string& serverHash);

    // Todo: refresh, validate, signout
};

} // ns Minecraft

#endif
