//////////////////////////////////////////////////////////////
// Server.h         - Server Base Class to create winsock   //
//                    based Server Application.             //
// Version          - 1.1                                   //
// Last Modified    - 08/06/2017                            //
// Language         - Visual C++, Visual Studio 2015        //
// Platform         - MSI GE62 2QD, Core-i7, Windows 10     //
// Author           - Venkata Bharani Krishna Chekuri       //
// e-mail           - bharanikrishna7@gmail.com             //
//////////////////////////////////////////////////////////////
/*
 * INFORMATION
 * -----------
 * This header provides a winsock based server interface for users to create
 * server applications. The goal of this header is to provide users a simple 
 * and easy to way to implement server type applications by simply inheriting
 * the server class and implementing response function which will do actual
 * processing and send back the response (if the developer desires) to the 
 * clients.
 *
 * Note : 
 * - The server is designed to create a server on localhost. But with
 * minor modifications it can be changed to run the server on some other 
 * machine provided the user has access to create and bind sockets to ports 
 * on other machine(s).
 * 
 * PACKAGE OPERATIONS
 * ------------------
 * - startServer(int port)
 * This method will start the server by binding the socket to a port on localhost.
 * 
 * - response(clientSocket, buffer, bufferSize)
 * This method takes in 3 arguments : 
 *	1> clientSocket	:= If the user wants to send some response back to client.
 *	2> buffer		:= The request which the user received from the client.
 *	3> bufferSize	:= The size of the request (in bytes).
 *
 * REQUIRED FILES
 * --------------
 * SocketCommons.h, Utilities.h, Utilities.cpp.
 *
 * OTHER DEPENDENCIES
 * ------------------
 * Platform : Requires Visual C++
 *
 * Maintenance History
 * -------------------
 * ver 1.0 : 08/06/2017
 * - First release.
 *
 * ver 1.1 : 08/07/2017
 * - Implemented Remote Exit Function to terminate server.
 * - Implemented Function to Close client Connection.
 * - Implemented Multiple clients to be able to establish connection with server.
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include "SocketCommons.h"

/* Abstract Class to create a server on localhost. */
class Server {
private:
	bool _terminate;
	fd_set _master;
protected:
	bool VERBOSE;

	bool terminateServerCheck(char* buffer) {
		if (Utilities::StringHelper::lrtrim(std::string(buffer)) == TERMINATE_SERVER_COMMAND) {
			_terminate = true;
			return true;
		}
		return false;
	}

	bool terminateClientCheck(char* buffer) {
		if (Utilities::StringHelper::lrtrim(std::string(buffer)) == TERMINATE_CLIENT_COMMAND)
			return true;
		return false;
	}

	virtual void response(SOCKET clientSocket, std::string buffer, int bufferSize) = 0;
public:
	/* Constructor */
	Server(bool verbose = false) {
		FD_ZERO(&_master);
		VERBOSE = verbose;
		_terminate = false;
	}
	
	/* Method to start the server on port specified in parameter. Else the server will be started on DEFAULT_PORT */
	void startServer(int port = DEFAULT_PORT) {
		// Initialize Winsock
		WSAData wsData;
		WORD version = MAKEWORD(2, 2);

		int wsStatus = WSAStartup(version, &wsData);

		if (wsStatus != 0) {
			std::cerr << "\n Cant Initialize Winsock !" << wsStatus << std::endl;
			return;
		}

		// Create Socket
		SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (listeningSocket == INVALID_SOCKET) {
			std::cerr << "\n Cant Create Socket" << std::endl;
			return;
		}

		// Bind ip and port to Socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));

		// Listen
		listen(listeningSocket, SOMAXCONN);

		FD_SET(listeningSocket, &_master);

		char buf[DEFAULT_BUFFER];
		while (true) {
			if (_terminate)
				break;
			fd_set masterCopy = _master;
			int socketCount = select(0, &masterCopy, nullptr, nullptr, nullptr);
			for (int i = 0; i < socketCount; i++) {
				SOCKET socks = masterCopy.fd_array[i];
				if (socks == listeningSocket) {
					// Wait for connection
					sockaddr_in client;
					int clientSize = sizeof(client);

					// Accept a new connection
					SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
					if (clientSocket == INVALID_SOCKET) {
						std::cerr << "\n Invalid Client Socket" << std::endl;
						continue;
					}

					std::cout << "\n New host connected with information : " << SocketUtilities::getClientInfo(clientSocket);
					// Add new connection to list of _master file descriptor set
					FD_SET(clientSocket, &_master);
					
					// Send Welcome Message to newly connected client.
					std::string welcomeMsg = " Welcome !\r\n";
					send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
				}
				else {
					// Accept a new request and respond
					ZeroMemory(buf, DEFAULT_BUFFER);
					// wait for client to send data
					int bytesReceived = recv(socks, buf, DEFAULT_BUFFER, 0);
					if (bytesReceived == SOCKET_ERROR) {
						std::cerr << "\n Error in recv()" << std::endl;
						continue;
					}
					/*
					// if client sends nothing then disconnect client
					if (bytesReceived <= 0) {
						std::cout << "\n Disconnecting Client : " << Utilities::Sockets::getClientInfo(socks) << std::endl;
						// disconnect client and remove it from the _master flie descriptor set
						closesocket(socks);
						FD_CLR(socks, &_master);
						continue;
					}
					*/
					if (terminateServerCheck(buf))
						break;
					
					if (terminateClientCheck(buf)) {
						closesocket(socks);
						FD_CLR(socks, &_master);
						break;
					}

					if (VERBOSE)
						std::cout << "\n RECV FROM CLIENT => " << SocketUtilities::getClientInfo(socks) << " ~ " << buf;
					// Respond to the client
					response(socks, buf, bytesReceived);
					// Can implement broadcast for Group Chat
				}
			}
		}

		terminateServer();
	}

	/* Method to terminate server */
	void terminateServer() {
		// close all sockets
		while(_master.fd_count > 0) {
			SOCKET socks = _master.fd_array[0];
			std::cout << "\n Closing Socket : " << SocketUtilities::getClientInfo(socks) << std::endl;
			closesocket(socks);
			FD_CLR(socks, &_master);
		}

		// Cleanup Winsock
		WSACleanup();
	}
};

#endif // !SERVER_H
