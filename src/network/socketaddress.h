#ifndef SOCKETADDRESS_H
#define SOCKETADDRESS_H

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdint>
#include <string>

namespace evnt
{
class SocketAddress
{
public:
    boost::asio::ip::udp::endpoint m_endpoint;

    SocketAddress() = default;
    SocketAddress(uint32_t inAddress, uint16_t inPort) :
        m_endpoint{boost::asio::ip::address_v4(inAddress), inPort}
    {}

    SocketAddress(std::string const & inAddress, std::string inPort) :
        m_endpoint(boost::asio::ip::address::from_string(inAddress),
                   boost::lexical_cast<unsigned short>(inPort))
    {}

    bool operator==(SocketAddress const & inOther) const { return m_endpoint == inOther.m_endpoint; }

    std::string    getAddress() const { return m_endpoint.address().to_string(); }
    unsigned short getPort() const { return m_endpoint.port(); }

    std::string toString() const;
};
}   // namespace evnt
#endif   // SOCKETADDRESS_H
