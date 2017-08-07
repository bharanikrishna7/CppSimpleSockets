#INTRODUCTION
This project provides simple and scalable Winsock2 C++ sockets implementation for developers. The motivation behind the implementation of this project was to help new C++ developers to very easily and quickly make their applications client server based architecture without much effort.

#KEY FEATURES
**CLIENT SOCKETS**
Client.h provides a simple static function which a developer can use to request and get response from a Winsock based server *(I have not tested this on other servers but the chances of it working with other servers are bleak)*.

**SERVER SOCKETS**
Server.h provides an abstract class which can be used to make a derived class Winsock based server. The developer just needs to implement the function ***response*** to process requests from clients. This allows developers to worry less on the Socket programming and more on their application features.

#UPDATE
* Implement multiple clients to be able to establish connection with server.
* Remote Exit Function for server i.e. Client can stop server with some command.
* Implement an exit function to for client to disconnect from server.
* Interface for client to send requests to server without taking argument from console (terminal / command prompt), just like an API.