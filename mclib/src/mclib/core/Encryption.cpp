#include <mclib/core/Encryption.h>

#include <mclib/common/DataBuffer.h>

#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <algorithm>
#include <functional>
#include <random>

namespace mc {
namespace core {

class RandomGenerator {
private:
    std::random_device m_RandomDevice;
    std::mt19937 m_Generator;

public:
    RandomGenerator() : m_RandomDevice(), m_Generator(m_RandomDevice()) {}

    RandomGenerator(unsigned int seed) : m_RandomDevice(), m_Generator(seed) {}

    unsigned int GetInt(unsigned int min, unsigned int max) {
        std::uniform_int_distribution<unsigned int> distr(min, max);
        return distr(m_Generator);
    }
};

DataBuffer EncryptionStrategyNone::Encrypt(const DataBuffer& buffer) {
    return buffer;
}

DataBuffer EncryptionStrategyNone::Decrypt(const DataBuffer& buffer) {
    return buffer;
}

class EncryptionStrategyAES::Impl {
private:
    RandomGenerator m_RNG;
    EVP_CIPHER_CTX* m_EncryptCTX;
    EVP_CIPHER_CTX* m_DecryptCTX;
    unsigned int m_BlockSize;

    protocol::packets::out::EncryptionResponsePacket* m_ResponsePacket;

    struct {
        unsigned char* key;
        unsigned int len;
    } m_PublicKey;

    struct {
        unsigned char key[AES_BLOCK_SIZE];
        unsigned int len;
    } m_SharedSecret;

    bool Initialize(const std::string& publicKey,
                    const std::string& verifyToken) {
        // Store received public key
        m_PublicKey.len = publicKey.length();
        m_PublicKey.key = new unsigned char[m_PublicKey.len];
        std::copy(publicKey.begin(), publicKey.end(), m_PublicKey.key);

        RSA* rsa = d2i_RSA_PUBKEY(NULL, (const unsigned char**)&m_PublicKey.key,
                                  m_PublicKey.len);

        // Generate random shared secret
        m_SharedSecret.len = AES_BLOCK_SIZE;
        std::generate(m_SharedSecret.key,
                      m_SharedSecret.key + m_SharedSecret.len,
                      std::bind(&RandomGenerator::GetInt, &m_RNG, 0, 255));

        int rsaSize = RSA_size(rsa);

        std::string encryptedSS;
        std::string encryptedToken;

        encryptedSS.resize(rsaSize);
        encryptedToken.resize(rsaSize);

        // Encrypt the shared secret with public key
        RSA_public_encrypt(AES_BLOCK_SIZE, m_SharedSecret.key,
                           (unsigned char*)&encryptedSS[0], rsa,
                           RSA_PKCS1_PADDING);
        // Encrypt the verify token with public key
        RSA_public_encrypt(
            verifyToken.length(), (const unsigned char*)verifyToken.c_str(),
            (unsigned char*)&encryptedToken[0], rsa, RSA_PKCS1_PADDING);
        RSA_free(rsa);

        // Initialize AES encryption and decryption
        if (!(m_EncryptCTX = EVP_CIPHER_CTX_new())) return false;

        if (!(EVP_EncryptInit_ex(m_EncryptCTX, EVP_aes_128_cfb8(), nullptr,
                                 m_SharedSecret.key, m_SharedSecret.key)))
            return false;

        if (!(m_DecryptCTX = EVP_CIPHER_CTX_new())) return false;

        if (!(EVP_DecryptInit_ex(m_DecryptCTX, EVP_aes_128_cfb8(), nullptr,
                                 m_SharedSecret.key, m_SharedSecret.key)))
            return false;

        m_BlockSize = EVP_CIPHER_block_size(EVP_aes_128_cfb8());

        m_ResponsePacket = new protocol::packets::out::EncryptionResponsePacket(
            encryptedSS, encryptedToken);
        return true;
    }

public:
    Impl(const std::string& publicKey, const std::string& verifyToken)
        : m_EncryptCTX(nullptr),
          m_DecryptCTX(nullptr),
          m_ResponsePacket(nullptr) {
        m_PublicKey.key = nullptr;
        Initialize(publicKey, verifyToken);
    }

    ~Impl() {
        if (m_ResponsePacket) delete m_ResponsePacket;

        EVP_CIPHER_CTX_free(m_EncryptCTX);
        EVP_CIPHER_CTX_free(m_DecryptCTX);

        m_EncryptCTX = nullptr;
        m_DecryptCTX = nullptr;
    }

    DataBuffer encrypt(const DataBuffer& buffer) {
        DataBuffer result;
        int size = 0;

        result.Resize(buffer.GetSize() + m_BlockSize);
        EVP_EncryptUpdate(m_EncryptCTX, &result[0], &size, &buffer[0],
                          buffer.GetSize());
        result.Resize(size);

        return result;
    }

    DataBuffer decrypt(const DataBuffer& buffer) {
        DataBuffer result;
        int size = 0;

        result.Resize(buffer.GetSize() + m_BlockSize);
        EVP_DecryptUpdate(m_DecryptCTX, &result[0], &size, &buffer[0],
                          buffer.GetSize());
        result.Resize(size);

        return result;
    }

    std::string GetSharedSecret() const {
        return std::string((char*)m_SharedSecret.key, m_SharedSecret.len);
    }

    protocol::packets::out::EncryptionResponsePacket* GenerateResponsePacket()
        const {
        return m_ResponsePacket;
    }
};

EncryptionStrategyAES::EncryptionStrategyAES(const std::string& publicKey,
                                             const std::string& verifyToken) {
    m_Impl = new Impl(publicKey, verifyToken);
}

EncryptionStrategyAES::~EncryptionStrategyAES() { delete m_Impl; }

DataBuffer EncryptionStrategyAES::Encrypt(const DataBuffer& buffer) {
    return m_Impl->encrypt(buffer);
}

DataBuffer EncryptionStrategyAES::Decrypt(const DataBuffer& buffer) {
    return m_Impl->decrypt(buffer);
}

std::string EncryptionStrategyAES::GetSharedSecret() const {
    return m_Impl->GetSharedSecret();
}

protocol::packets::out::EncryptionResponsePacket*
EncryptionStrategyAES::GenerateResponsePacket() const {
    return m_Impl->GenerateResponsePacket();
}

}  // namespace core
}  // namespace mc
