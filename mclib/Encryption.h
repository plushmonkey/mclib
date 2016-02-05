#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <string>

#include "mclib.h"
#include "Packets/Packet.h"

namespace Minecraft {

class DataBuffer;

struct EncryptionStrategy {
    virtual ~EncryptionStrategy() { }
    virtual DataBuffer Encrypt(const DataBuffer& buffer) = 0;
    virtual DataBuffer Decrypt(const DataBuffer& buffer) = 0;
};

class EncryptionStrategyNone : public EncryptionStrategy {
public:
    DataBuffer MCLIB_API Encrypt(const DataBuffer& buffer);
    DataBuffer MCLIB_API Decrypt(const DataBuffer& buffer);
};

class EncryptionStrategyAES : public EncryptionStrategy {
private:
    class Impl;
    Impl* m_Impl;

    EncryptionStrategyAES(const EncryptionStrategyAES& other);
    EncryptionStrategyAES& operator=(const EncryptionStrategyAES& other);

public:
    MCLIB_API EncryptionStrategyAES(const std::string& publicKey, const std::string& verifyToken);
    MCLIB_API ~EncryptionStrategyAES();

    DataBuffer MCLIB_API Encrypt(const DataBuffer& buffer);
    DataBuffer MCLIB_API Decrypt(const DataBuffer& buffer);

    std::string MCLIB_API GetSharedSecret() const;
    MCLIB_API Packets::Outbound::EncryptionResponsePacket* GenerateResponsePacket() const;
};

}

#endif
