#ifdef TEST_SOCKETS

#include <thread>
#include <iostream>

#include "Client.h"
#include "Server.h"
#include "SocketCommons.h"

using namespace Utilities;

/* Simple Implementation of Server Class for Testing Purposes */
class ServerApp : public Server {
protected:
	void response(SOCKET clientSocket, std::string buffer, int bufferSize) {
		// echo message back to client
		std::cout << " SERVER RESPONSE : > " << buffer;
		send(clientSocket, buffer.c_str(), bufferSize + 1, 0);
	}
public:
	ServerApp(int port = DEFAULT_PORT, bool verbose = false) {
		VERBOSE = verbose;
	}
};

/* Method to Initialize and Run Server. Ideally should be called in a separate thread. */
void runServer(ServerApp * server) {
	server = new ServerApp(8081, true);
	server->startServer();
}

int main() {
	ServerApp * server;
	std::thread serverThread(runServer, std::ref(server));
	
	std::string result;

	Client::Connect(result,"127.0.0.1", 8081, "Mom I'm on T.V. !!!");
	Client::Connect(result);
	serverThread.join();
	
	std::cout << "\n\n ";
}

#endif // TEST_SOCKETS
