#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <string>

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
    DataBuffer Encrypt(const DataBuffer& buffer);
    DataBuffer Decrypt(const DataBuffer& buffer);
};

class EncryptionStrategyAES : public EncryptionStrategy {
private:
    class Impl;
    Impl* m_Impl;

    EncryptionStrategyAES(const EncryptionStrategyAES& other);
    EncryptionStrategyAES& operator=(const EncryptionStrategyAES& other);

public:
    EncryptionStrategyAES(const std::string& publicKey, const std::string& verifyToken);
    ~EncryptionStrategyAES();

    DataBuffer Encrypt(const DataBuffer& buffer);
    DataBuffer Decrypt(const DataBuffer& buffer);

    std::string GetSharedSecret() const;
    Packets::Outbound::EncryptionResponsePacket* GenerateResponsePacket() const;
};

}

#endif
