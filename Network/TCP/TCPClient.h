#ifndef TCPClient_H
#define TCPClient_H

#include <SFML/Network.hpp>

#include <iostream>
#include <string>

enum class ConnectionState { NOAUTH, AUTH_OK };

class TCPClient
{
protected:
	sf::TcpSocket server_socket; // A TCP socket designated for the server
	unsigned long guuid; // Global Unique User Identification received from server
	ConnectionState connection_state; // Overall state of the connection
	sf::IpAddress server_address; // Remote IP address we are connected to
	unsigned short server_port; // Remote port we are connected to

	virtual bool Authenticate(sf::TcpSocket& s);

public:
	TCPClient(sf::IpAddress addr, unsigned short port);
	virtual bool Receive(sf::Packet& p);
	virtual bool Send(sf::Packet& p);
	virtual void Disconnect();
	virtual ~TCPClient();
};

#endif