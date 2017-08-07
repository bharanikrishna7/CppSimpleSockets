#ifdef TEST_SOCKETS

#include <thread>
#include <iostream>

#include "Client.h"
#include "Server.h"
#include "../Utilities/Utilities.h"

using namespace Utilities;

/* Simple Implementation of Server Class for Testing Purposes */
class ServerApp : public Server {
protected:
	void response(SOCKET clientSocket, std::string buffer, int bufferSize) {
		// echo message back to client
		std::cout << "\n [SERVER RECEIVED] : > " << buffer;
		send(clientSocket, buffer.c_str(), bufferSize + 1, 0);
	}
public:
	ServerApp(int port = DEFAULT_PORT, bool verbose = false) {
		//::Server(port, verbose);
	}
};

/* Method to Initialize and Run Server. Ideally should be called in a separate thread. */
void runServer(ServerApp * server) {
	server = new ServerApp();
	server->startServer();
}

int main() {
	ServerApp * server;
	std::thread serverThread(runServer, std::ref(server));
	
	std::string result = "n/a";

	Client::Connect(result);
	std::cout << "\n SERVER RESPONSE : " << result;

	serverThread.join();

	std::cout << "\n\n ";
}

#endif // TEST_SOCKETS
