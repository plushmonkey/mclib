#ifndef NETWORK_IPADDRESS_H_
#define NETWORK_IPADDRESS_H_

#include "../Types.h"

#include <string>
#include <iosfwd>
#include <vector>

namespace Network {

/* IPv4 address */
class IPAddress {
private:
    u32 m_Address;
    bool m_Valid;

public:
    /* Create an invalid address */
    IPAddress();

    /* Initialize by string IP */
    IPAddress(const std::string& str);

    /* Initialize by string IP */
    IPAddress(const std::wstring& str);

    /* Initialize by octets */
    IPAddress(u8 octet1, u8 octet2, u8 octet3, u8 octet4);

    /* Get the specific octet. 1-4 */
    u8 GetOctet(u8 num) const;

    /* Set the specific octet. 1-4 */
    void SetOctet(u8 num, u8 value);

    /* Make sure the IP is valid. It will be invalid if the host wasn't found. */
    bool IsValid() const { return m_Valid; }

    std::string ToString() const;

    static IPAddress LocalAddress();

    bool operator==(const IPAddress& right);
    bool operator!=(const IPAddress& right);
    bool operator==(bool b);
};

typedef std::vector<IPAddress> IPAddresses;

std::ostream& operator<<(std::ostream& os, const IPAddress& addr);
std::wostream& operator<<(std::wostream& os, const IPAddress& addr);

} // ns Network

#endif
