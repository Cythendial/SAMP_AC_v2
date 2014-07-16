#pragma once

#include "../../Shared/RakNet/RakPeer.h"
#include "../../Shared/RakNet/MessageIdentifiers.h"
#include "../../Shared/Network/Network.h"

class CRakServer
{
public:
	CRakServer();
	virtual ~CRakServer();

	RakNet::StartupResult Startup(const char* szHostAddress, t_port usPort, unsigned short iConnections);
	void Shutdown(unsigned int blockDuration, PacketPriority priority);
	unsigned int Send(Network::ePacketType packetType, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F, bool bBroadcast = false);
	unsigned int SendRPC(unsigned short usRPCId, const RakNet::SystemAddress& systemAddress, RakNet::BitStream* pBitStream = NULL, PacketPriority priority = HIGH_PRIORITY, PacketReliability reliability = RELIABLE_ORDERED, char cOrderingChannel = 0x7F, bool bBroadcast = false);

	RakNet::Packet* Receive();
	void DeallocatePacket(RakNet::Packet* pPacket);
	void CloseConnection(const RakNet::SystemAddress& pSystemAddress);
	std::string GetMyIP();

private:
	RakNet::RakPeer* m_pPeer;
	RakNet::SocketDescriptor* m_pSocketDescriptor;

};