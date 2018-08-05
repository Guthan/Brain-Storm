#include "TCPClient.h"

bool TCPClient::Authenticate(sf::TcpSocket& s) // Function to authenticate the client with the server
{
	sf::Packet p; // Create a packet to receive information in
	sf::Uint64 id; // Create a variable to store potential guuid
	sf::Socket::Status status = server_socket.receive(p); // Attempt to receive data
	if(status == sf::Socket::Status::Done) // If we received data
	{
		if(!(p >> id)) return false; // Return unsucessful if we can't get the guuid
		p.clear(); // Empty the packet
		p << id;// Put our guuid in the packet to confirm
		status = server_socket.send(p); // Attempt to send the packet
		if(status == sf::Socket::Done) // If the guuid confirmation was sent
		{
			p.clear(); // Empty the packet for new information
			sf::Socket::Status status = server_socket.receive(p); // Attempt to receive authentication confirmation
			if(status == sf::Socket::Status::Done) // If we received data
			{
				std::string response = ""; // Make a place to store the response
				if(p >> response) // If we can get data
				{
					if(response == "AUTH_OK") // Last piece of our authentication is this string being sent to the client
					{
						connection_state = ConnectionState::AUTH_OK; // Update the internal connection state
						guuid = id; // We now have a valid GUUID to talk with the server
						return true; // Data is what intended we are authenticated!
					}
				}
			}
		}
	}
	guuid = 0; // We do not yet have a valid GUUID
	return false; // Something went wrong with authentication
}

TCPClient::TCPClient(sf::IpAddress addr, unsigned short port)
: server_address(addr), server_port(port), connection_state(ConnectionState::NOAUTH)
{
	sf::Socket::Status status = server_socket.connect(server_address, server_port, sf::seconds(10));
	if(status == sf::Socket::Status::Done)
	{
		if(Authenticate(server_socket))
		{
			std::cout << "Authentication successful.\n";
		}
	}
}

bool TCPClient::Receive(sf::Packet& p)
{
	if(connection_state == ConnectionState::NOAUTH) return false;
	sf::Socket::Status status;
	status = server_socket.receive(p);
	while(status == sf::Socket::Status::Partial)
		status = server_socket.receive(p);
	if(status == sf::Socket::Status::Done)
		return true;
	return false;
}

bool TCPClient::Send(sf::Packet& p)
{
	if(connection_state == ConnectionState::NOAUTH) return false;
	sf::Packet data;
	data << (sf::Uint64)guuid; // We send out our guuid first so the server knows who it's dealing with
	const void* p_ptr = p.getData();
	size_t len = p.getDataSize();
	data.append(p_ptr, len);
	sf::Socket::Status status;
	status = server_socket.send(p);
	while(status == sf::Socket::Status::Partial)
		status = server_socket.send(p);
	if(status == sf::Socket::Status::Done)
		return true;
	return false;
}

void TCPClient::Disconnect()
{
	sf::Packet p;
	p << (sf::Uint64)guuid;
	p << "DISC_REQ";
	Send(p);
	server_socket.disconnect();
	connection_state = ConnectionState::NOAUTH;
}

TCPClient::~TCPClient()
{
	Disconnect();
}