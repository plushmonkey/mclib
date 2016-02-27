#ifndef HASH_H_
#define HASH_H_

#include <array>
#include <string>
#include "mclib.h"

std::string MCLIB_API Base64Decode(const std::string& message);

namespace Sha {

#define DIGEST_LENGTH 20


/**
* Calculates the two's complement of a sha1 digest.
* @param digest The original sha1 digest.
* @return an array that contains the two's complement of the digest.
*/
std::array<unsigned char, DIGEST_LENGTH> MCLIB_API TwosComplement(const unsigned char* digest);

/**
* Gets the hex representation of a sha1 digest.
* Uses two's complement for negative digests, and removes leading zeros.
* @param digest The raw sha1 digest
* @return a string containing the hex digest
*/
std::string MCLIB_API Sha1HexDigest(const unsigned char* digest);
bool MCLIB_API Sha1DigestTest();

}

#endif
