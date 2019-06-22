#ifndef MCLIB_UTIL_HASH_H_
#define MCLIB_UTIL_HASH_H_

#include <mclib/mclib.h>

#include <array>
#include <string>

namespace mc {
namespace util {

std::string MCLIB_API Base64Decode(const std::string& message);

#define SHA1_DIGEST_LENGTH 20

/**
 * Calculates the two's complement of a sha1 digest.
 * @param digest The original sha1 digest.
 * @return an array that contains the two's complement of the digest.
 */
std::array<unsigned char, SHA1_DIGEST_LENGTH> MCLIB_API
Sha1TwosComplement(const unsigned char* digest);

/**
 * Gets the hex representation of a sha1 digest.
 * Uses two's complement for negative digests, and removes leading zeros.
 * @param digest The raw sha1 digest
 * @return a string containing the hex digest
 */
std::string MCLIB_API Sha1HexDigest(const unsigned char* digest);
bool MCLIB_API Sha1DigestTest();

}  // namespace util
}  // namespace mc

#endif
