#ifndef TCPServer_H
#define TCPServer_H

#include <SFML/Network.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <utility>

class TCPServer
{
protected:
	unsigned short listening_port;
	sf::TcpListener listener;
	sf::SocketSelector selector;
	std::map<unsigned long, std::unique_ptr<sf::TcpSocket>> clients;
	unsigned long guuid;

	virtual bool Authenticate(std::unique_ptr<sf::TcpSocket>& socket);

public:
	TCPServer(unsigned short port);
	virtual bool AcceptConnection();
	virtual bool ReceivePackets() = 0;
	virtual bool SendPacket(unsigned long id, sf::Packet& p);
};

#endif