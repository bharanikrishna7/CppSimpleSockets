//////////////////////////////////////////////////////////////////
// SocketCommons.h  - This class contains all the common        //
//                    things which Server.h & Client.h use.     //
// Version          - 1.0 (final)                               //
// Last Modified    - 08/06/2017                                //
// Language         - Visual C++, Visual Studio 2017            //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10         //
// Author           - Venkata Bharani Krishna Chekuri           //
// e-mail           - bharanikrishna7@gmail.com                 //
//////////////////////////////////////////////////////////////////
/*
 * INFORMATION
 * -----------
 * Contains all the common imports and defines which both the Client.h
 * and Server.h use. It also contains Static Utility Functions for 
 * the Client and Server Sockets.
 * 
 * REQUIRED FILES
 * --------------
 * Utilities.h, Utilities.cpp
 * 
 * OTHER DEPENDENCIES
 * ------------------
 * Platform : Requires Visual C++
 *
 * CHANGELOG
 * ---------
 * ver 1.0 : 08/07/2017
 * - First release.
 *
 */
#ifndef SOCKETCOMMONS_H
#define SOCKETCOMMONS_H

#include <iostream>
#include <WS2tcpip.h>

/* Easier to do it here that add "Ws2_32.lib" in linker. */
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT 8081				// Default Port to Initialize Server
#define DEFAULT_BUFFER 18000			// Default Message Buffer Size.
#define DEFAULT_IP "127.0.0.1"			// Default IP Address where the Client Connects.

/* Server Commands To Terminate Connections */
#define TERMINATE_CLIENT_COMMAND "termClient();"
#define TERMINATE_SERVER_COMMAND "termServer();"

#include "Utilities.h"

/// <summary>
/// Class containing Static Utility Functions for Client and Server Sockets.
/// </summary>
class SocketUtilities {
public:
	static std::string getClientInfo(SOCKET& client);
};

/// <summary>
/// Function to retrieve IP Address and Port information from a SOCKET
/// </summary>
/// <param name="client">Client Socket</param>
/// <returns>Address and Port associated with Supplied Socket</returns>
std::string SocketUtilities::getClientInfo(SOCKET& client) {
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;

	len = sizeof addr;
	getpeername(client, (struct sockaddr*)&addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET) {
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else { // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	std::string address = ipstr;
	address.push_back(':');
	address += std::to_string(port);
	return address;
}

#endif // !SOCKETCOMMONS_H
