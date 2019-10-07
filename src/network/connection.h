#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "../core/memory_stream.h"
#include "udpsocket.h"
#include <list>
#include <queue>

namespace evnt
{
class Connection
{
public:
    static const int kMaxPacketsPerFrameCount = 10;

    Connection();
    virtual ~Connection() = default;

    bool init(uint16_t inPort);
    void processIncomingPackets();

    virtual void processPacket(InputMemoryStream & inInputStream, const SocketAddress & inFromAddress) = 0;
    virtual void handleConnectionReset(const SocketAddress & inFromAddress) { (void)inFromAddress; }

    void sendPacket(const OutputMemoryStream & inOutputStream, const SocketAddress & inToAddress);

private:
    // void	UpdateBytesSentLastFrame();
    void readIncomingPacketsIntoQueue();
    void processQueuedPackets();

    class ReceivedPacket
    {
    public:
        ReceivedPacket(uint32_t inReceivedTime, InputMemoryStream & inInputMemoryStream,
                       const SocketAddress & inAddress) :
            m_received_time{inReceivedTime}, m_packet_buffer{inInputMemoryStream}, m_from_address{inAddress}
        {}

        const SocketAddress & getFromAddress() const { return m_from_address; }
        uint32_t              getReceivedTime() const { return m_received_time; }
        InputMemoryStream &   getPacketBuffer() { return m_packet_buffer; }

    private:
        uint32_t          m_received_time;
        InputMemoryStream m_packet_buffer;
        SocketAddress     m_from_address;
    };

    std::queue<ReceivedPacket, std::list<ReceivedPacket>> m_packet_queue;
    UDPSocketPtr                                          m_socket;
};
}   // namespace evnt

#endif   // NETWORKMANAGER_H
