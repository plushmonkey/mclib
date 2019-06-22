#ifndef NETWORK_IPADDRESS_H_
#define NETWORK_IPADDRESS_H_

#include <mclib/common/Types.h>
#include <iosfwd>
#include <string>
#include <vector>

namespace mc {
namespace network {

/* IPv4 address */
class IPAddress {
private:
    u32 m_Address;
    bool m_Valid;

public:
    /* Create an invalid address */
    MCLIB_API IPAddress() noexcept;

    /* Initialize by string IP */
    MCLIB_API IPAddress(const std::string& str);

    /* Initialize by string IP */
    MCLIB_API IPAddress(const std::wstring& str);

    /* Initialize by octets */
    MCLIB_API IPAddress(u8 octet1, u8 octet2, u8 octet3, u8 octet4) noexcept;

    /* Get the specific octet. 1-4 */
    u8 MCLIB_API GetOctet(u8 num) const;

    /* Set the specific octet. 1-4 */
    void MCLIB_API SetOctet(u8 num, u8 value);

    /* Make sure the IP is valid. It will be invalid if the host wasn't found.
     */
    bool IsValid() const noexcept { return m_Valid; }

    std::string MCLIB_API ToString() const;

    static IPAddress MCLIB_API LocalAddress();

    MCLIB_API bool operator==(const IPAddress& right);
    MCLIB_API bool operator!=(const IPAddress& right);
    MCLIB_API bool operator==(bool b);
};

typedef std::vector<IPAddress> IPAddresses;

MCLIB_API std::ostream& operator<<(std::ostream& os, const IPAddress& addr);
MCLIB_API std::wostream& operator<<(std::wostream& os, const IPAddress& addr);

}  // namespace network
}  // namespace mc

#endif
