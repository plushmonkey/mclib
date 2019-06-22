#include <mclib/network/IPAddress.h>

#include <regex>
#include <sstream>
#include <stdexcept>

namespace {

const std::regex IPRegex(R":(^([0-9]+)\.([0-9]+)\.([0-9]+)\.([0-9]+)$):");
const std::wregex IPRegexW(LR":(^([0-9]+)\.([0-9]+)\.([0-9]+)\.([0-9]+)$):");

}  // namespace

namespace mc {
namespace network {

/* Create an invalid address */
IPAddress::IPAddress() noexcept : m_Valid(false), m_Address(0) {}

/* Initialize by string IP */
IPAddress::IPAddress(const std::string& ip) : m_Valid(false), m_Address(0) {
    std::sregex_iterator begin(ip.begin(), ip.end(), IPRegex);
    std::sregex_iterator end;

    if (begin == end) return;  // m_Valid = false

    std::smatch match = *begin;

    int octet1 = atoi(std::string(match[1]).c_str());
    int octet2 = atoi(std::string(match[2]).c_str());
    int octet3 = atoi(std::string(match[3]).c_str());
    int octet4 = atoi(std::string(match[4]).c_str());

    m_Address = (octet1 << 24) | (octet2 << 16) | (octet3 << 8) | octet4;
    m_Valid = true;
}

IPAddress::IPAddress(const std::wstring& ip) : m_Address(0), m_Valid(false) {
    std::wsregex_iterator begin(ip.begin(), ip.end(), IPRegexW);
    std::wsregex_iterator end;

    if (begin == end) return;  // m_Valid = false

    std::wsmatch match = *begin;

    int octet1 = std::stoi(match[1]);
    int octet2 = std::stoi(match[2]);
    int octet3 = std::stoi(match[3]);
    int octet4 = std::stoi(match[4]);

    m_Address = (octet1 << 24) | (octet2 << 16) | (octet3 << 8) | octet4;
    m_Valid = true;
}

/* Initialize by octets */
IPAddress::IPAddress(uint8_t octet1, uint8_t octet2, uint8_t octet3,
                     uint8_t octet4) noexcept
    : m_Valid(true) {
    m_Address = (octet1 << 24) | (octet2 << 16) | (octet3 << 8) | octet4;
}

/* Get the specific octet */
uint8_t IPAddress::GetOctet(uint8_t num) const {
    if (num == 0 || num > 4)
        throw std::invalid_argument("Invalid argument in IPAddress:GetOctet.");

    return (m_Address >> (8 * (4 - num))) & 0xFF;
}

/* Set the specific octet. 1-4 */
void IPAddress::SetOctet(uint8_t num, uint8_t value) {
    if (num == 0 || num > 4)
        throw std::invalid_argument("Invalid argument in IPAddress:GetOctet.");
    uint8_t octets[4];

    for (int i = 0; i < 4; ++i) octets[i] = (m_Address >> ((3 - i) * 8)) & 0xFF;

    octets[num - 1] = value;

    m_Address =
        (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
}

IPAddress IPAddress::LocalAddress() { return IPAddress(127, 0, 0, 1); }

std::string IPAddress::ToString() const {
    std::stringstream ss;

    for (int i = 0; i < 4; ++i) {
        if (i != 0) ss << ".";
        ss << static_cast<unsigned int>(GetOctet(i + 1));
    }

    return ss.str();
}

bool IPAddress::operator==(const IPAddress& right) {
    return m_Address == right.m_Address;
}

bool IPAddress::operator!=(const IPAddress& right) { return !(*this == right); }

bool IPAddress::operator==(bool b) { return IsValid() == b; }

std::ostream& operator<<(std::ostream& os, const IPAddress& addr) {
    return os << addr.ToString();
}

std::wostream& operator<<(std::wostream& os, const IPAddress& addr) {
    std::string str = addr.ToString();
    return os << std::wstring(str.begin(), str.end());
}

}  // namespace network
}  // namespace mc
