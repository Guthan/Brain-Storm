#include "TCPServer.h"

bool TCPServer::Authenticate(std::unique_ptr<sf::TcpSocket>& socket) // Function attempts to authenticate a new client connection
{
	sf::Packet id; // Create a packet to send the guuid and get responses with
	id << (sf::Uint64)guuid; // Load the guuid into the packet
	sf::Socket::Status status = socket->send(id); // Send the packet
	while(status == sf::Socket::Status::Partial) // While only part of the packet was sent
		status = socket->send(id); // Keep attempting send
	if(status == sf::Socket::Status::Done) // If the guuid send was sucessful
	{
		id.clear(); // Clear the packet of data ready for new information
		status = socket->receive(id); // Attempt to receive response from client
		while(status == sf::Socket::Status::Partial) // While only part of the packet was received
			status = socket->receive(id); // Keep attempting to receive
		if(status == sf::Socket::Status::Done) // If receive was sucessful
		{
			sf::Uint64 check = 0; // Create a variable to store guuid response
			if(id >> check) // If attempt to extact guuid from packet was sucessful
			{
				if(check == (sf::Uint64)guuid) // If response matches guuid that was sent
				{
					id.clear(); // Clear the packet ready for new data
					id << "AUTH_OK"; // Load the all clear response
					status = socket->send(id); // Attempt to send the all clear
					while(status == sf::Socket::Status::Partial) // While only part of the packet was sent
						status = socket->send(id); // Attempt to send the packet
					if(status == sf::Socket::Status::Done) // If the all clear was sent
					{
						clients[guuid] = std::move(socket); // Add the guuid and socket to the map
						selector.add(*clients[guuid]); // Add trusted socket to the selector
						guuid++; // Increase the guuid ready for the next connection
						return true; // Socket authentication sucessful
					}
					// Failed to send Authentication success
				}
				// GUUID MISMATCH
			}
			// Client Authentication response is of wrong type
		}
		// Could not retrieve client response
	}
	// Could not send GUUID to client
	return false; // Socket authentication not sucessful
}

TCPServer::TCPServer(unsigned short port): listening_port(port), guuid(1)
{
	if(listener.listen(port) != sf::Socket::Status::Done) // If we can't start listening for connections
	{
		// Could not start the TCP service
	}
	else
	{
		selector.add(listener); // Add the listener to the selector
		// TCP service started
	}
}

bool TCPServer::AcceptConnection() // Function attempts to gain a new authenticated connection
{
	if(selector.wait(sf::seconds(0))) // Check if any sockets in the selector have any pending inbound data
	{
		if(selector.isReady(listener)) // If our listener is ready make a socket for connection
		{
			std::unique_ptr<sf::TcpSocket> socket;
			if(listener.accept(*socket)) // If we accepted the connection attempt an authentication
			{
				if(Authenticate(socket))
				{
					// Authenticated connection
					return true;
				}
				else
				{
					//Rejecting un-authenticated client
				}
			}
			// No pending connection
		}
		// Socket not ready
	}
	return false;
}

bool TCPServer::SendPacket(unsigned long id, sf::Packet& p) // Function sends a packet to a known guuid
{
	if(clients.find(id) == clients.end()) return false; // If id is not in the list return unsucessful
	sf::Socket::Status status = clients[id]->send(p); // Send the packet
	while(status == sf::Socket::Status::Partial) // While only part of the packet was sent
		status = clients[id]->send(p); // Keep attempting send
	if(status == sf::Socket::Status::Done) // If packet was sent return sucessful
		return true;
	return false;
}