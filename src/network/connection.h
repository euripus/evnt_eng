#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "../fs/memory_stream.h"
#include "udpsocket.h"
#include <list>
#include <queue>

namespace evnt
{
class Connection
{
public:
    static int32_t const MAX_PACKETS_PERFRAME = 10;

    Connection();
    virtual ~Connection() = default;

    bool init(uint16_t inPort);
    void processIncomingPackets();

    virtual void processPacket(InputMemoryStream & inInputStream, SocketAddress const & inFromAddress) = 0;
    virtual void handleConnectionReset(SocketAddress const & inFromAddress) { (void)inFromAddress; }

    void sendPacket(OutputMemoryStream const & inOutputStream, SocketAddress const & inToAddress);

private:
    void readIncomingPacketsIntoQueue();
    void processQueuedPackets();

    class ReceivedPacket
    {
    public:
        ReceivedPacket(uint32_t inReceivedTime, InputMemoryStream & inInputMemoryStream,
                       SocketAddress const & inAddress) :
            m_received_time{inReceivedTime}, m_packet_buffer{inInputMemoryStream}, m_from_address{inAddress}
        {}

        SocketAddress const & getFromAddress() const { return m_from_address; }
        uint32_t              getReceivedTime() const { return m_received_time; }
        InputMemoryStream &   getPacketBuffer() { return m_packet_buffer; }

    private:
        uint32_t          m_received_time;
        InputMemoryStream m_packet_buffer;
        SocketAddress     m_from_address;
    };

    std::queue<ReceivedPacket, std::list<ReceivedPacket>> m_packet_queue;
    UDPSocketPtr                                          msp_socket;
};
}   // namespace evnt

#endif   // NETWORKMANAGER_H
