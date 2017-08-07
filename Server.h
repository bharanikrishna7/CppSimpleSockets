//////////////////////////////////////////////////////////////
// Server.h			-	Server Base Class to create winsock	//
//						based Server Application.			//
// Version			-	1.0 (final)							//
// Last Modified	-	08/06/2017							//
// Language			-	Visual C++, Visual Studio 2015		//
// Platform			-	MSI GE62 2QD, Core-i7, Windows 10	//
// Author			-	Venkata Bharani Krishna Chekuri		//
// e-mail			-	bharanikrishna7@gmail.com			//
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
 * 1> The server is designed to create a server on localhost. But with
 * minor modifications it can be changed to run the server on some other 
 * machine provided the user has access to create and bind sockets to ports 
 * on other machine(s).
 * 
 * 2> At the moment server can connect to only 1 client and once the connection
 * is terminated from client the server stops. This will be changed in next update
 * to allow multiple clients to connect to server and server will be stopped only
 * when the terminate method is called.
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
 * None.
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
 * TO-DO
 * -----
 * 1. Implement multiple clients to be able to establish connection with server.
 * 2. Remote Exit Function for server i.e. Client can stop server with some command.
 *
 * SHOUTOUT
 * --------
 * Huge shoutout to Prof. Jim Fawcett (Syracuse University) for teaching me everything
 * about Software Development, C++ and Design patterns. Huge props to him.
 *
 */
#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <WS2tcpip.h>

// Easier to do it here that add "Ws2_32.lib" in linker.
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT 8081
#define DEFAULT_BUFFER 18000

/* Abstract Class to create a server on localhost. */
class Server {
private:
	bool _terminate;
	bool VERBOSE;
protected:
	virtual void response(SOCKET clientSocket, std::string buffer, int bufferSize) = 0;
public:
	/* Constructor */
	Server(bool verbose = false) {
		_terminate = false;
		VERBOSE = verbose;
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
		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == INVALID_SOCKET) {
			std::cerr << "\n Cant Create Socket" << std::endl;
			return;
		}

		// Bind ip and port to Socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		hint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(serverSocket, (sockaddr*)&hint, sizeof(hint));

		// Listen
		listen(serverSocket, SOMAXCONN);

		// Wait for connection
		sockaddr_in client;
		int clientSize = sizeof(client);

		SOCKET clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << "\n Invalid Client Socket" << std::endl;
			return;
		}

		char host[NI_MAXHOST];				// Client's remote name.
		char service[NI_MAXSERV];			// Service (i.e port) client is connected on.

		ZeroMemory(host, NI_MAXHOST);		// same as memset(host, 0, NI_MAXHOST)
		ZeroMemory(service, NI_MAXSERV);	// same as memset(service, 0, NI_MAXHOST)

		if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
			std::cout << "\n Host connected on port " << service;
		}
		else {
			inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
			std::cout << "\n Host connected on port " << ntohs(client.sin_port);
		}

		// Close Listening Socket
		closesocket(serverSocket);

		// While Loop : Accept and echo message back to client (optional echo message)
		char buf[DEFAULT_BUFFER];
		while (!_terminate) {
			ZeroMemory(buf, DEFAULT_BUFFER);
			// wait for client to send data
			int bytesReceived = recv(clientSocket, buf, DEFAULT_BUFFER, 0);
			if (bytesReceived == SOCKET_ERROR) {
				std::cerr << "\n Error in recv()" << std::endl;
				_terminate = true;
				break;
			}

			if (bytesReceived == 0) {
				std::cout << "\n Client Disconnected" << std::endl;
				break;
			}
			if (VERBOSE)
				std::cout << "\n Recv Buf : " << buf;

			response(clientSocket, buf, bytesReceived);
		}

		// Close the Socket
		closesocket(clientSocket);

		// Cleanup Winsock
		WSACleanup();
	}

	/* Method to terminate server */
	void terminateServer() {
		_terminate = true;

		// Cleanup Winsock
		WSACleanup();
	}
};

#endif // !SERVER_H
