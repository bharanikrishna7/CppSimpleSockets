//////////////////////////////////////////////////////////////
// Client.h			-	Client Class to Connect and Recieve	//
//						response from Winsock based Server.	//
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
 * This header provides a static method to connect to and get response
 * from a Winsock based server (have not tested on GNU sockets server
 * but there's a bleak chance that it'll work on GNU socket server wit-
 * -hout modifications).
 *
 * The whole goal of this header is to provide users a simple and easy to 
 * use interface to query a winsock based server without having to implement 
 * the client sockets in their application class(es).
 *
 * PACKAGE OPERATIONS
 * ------------------
 * - Connect(result, ip, port, verbose)
 * The user can connect and perform request on a server at ip:port using this
 * method. The response from the server is stored in the "result" string which
 * user pases as an argument.
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
 * ------
 * 1. Implement an exit function to for client to disconnect from server.
 * 2. Interface for client to send requests to server without taking argument
 *    from console (terminal / command prompt).
 *
 * SHOUTOUT
 * --------
 * Huge shoutout to Prof. Jim Fawcett (Syracuse University) for teaching me everything
 * about Software Development, C++ and Design patterns. Huge props to him.
 *  
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <iostream>
#include <WS2tcpip.h>

// Easier to do it here that add "Ws2_32.lib" in linker.
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8081
#define DEFAULT_BUFFER 18000

class Client {
public:
	Client() {
		// do nothing
	}

	static void Connect(std::string& result, std::string ip = DEFAULT_IP, int port = DEFAULT_PORT, bool verbose = false) {
		result.clear();
		// Initialize Winsock
		WSAData wsData;
		WORD version = MAKEWORD(2, 2);
		int wsStatus = WSAStartup(version, &wsData);
		if (wsStatus != 0) {
			std::cerr << "\n Cant Initialize Winsock ! Err #" << wsStatus << std::endl;
			return;
		}

		// Create Socket
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (clientSocket == INVALID_SOCKET) {
			std::cerr << "\n Cant Create Socket. Err #" << WSAGetLastError() << std::endl;
			WSACleanup();
			return;
		}

		// Fill in hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &hint.sin_addr.S_un.S_addr);

		// Connect to server
		int connResult = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR) {
			std::cerr << "\n Can't connect to Server " << ip << ":" << port << ". Err #" << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			return;
		}

		// Do while loop to send and recv data
		std::string userInput;
		char buf[DEFAULT_BUFFER];
		do {
			Sleep(100);
			// Create a prompt
			std::cout << "\n > ";
			std::getline(std::cin, userInput);
			if (userInput.size() > 0) {
				// Send Text
				int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR) {
					// wait for response
					ZeroMemory(buf, DEFAULT_BUFFER);
					int bytesReceived = recv(clientSocket, buf, 4096, 0);
					if (bytesReceived > 0) {
						result += buf;
						// Echo response to console
						if(verbose)
							std::cout << "SERVER > " << std::string(buf, 0, bytesReceived);
					}
				}
			}
		} while (userInput.size() > 0);



		// Close down everything
		closesocket(clientSocket);
		WSACleanup();
	}
};

#endif // !CLIENT_H