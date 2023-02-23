#ifndef NET__
#define NET__

#pragma region Platform detection

#if defined(_WIN32) || defined(_WIN64)
	#ifndef PLATFORM_WINDOWS
		#define PLATFORM_WINDOWS
	#endif
#elif (defined(__APPLE__) && defined(__MACH__))
	#define PLATFORM_MAC
#else
	#define PLATFORM_UNIX
#endif

#pragma endregion /* Platform detection */

#if defined(PLATFORM_UNIX) || defined(PLATFORM_MAC)

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#elif defined(PLATFORM_WINDOWS)

#include <winsock2.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <thread>
#include<chrono>
#include "serialise.h"

#if defined(PLATFORM_UNIX) || defined(PLATFORM_MAC)

class Server
{
    private:
        unsigned short portno;
        int server_socket;
        int new_socket, err;
        socklen_t client_len;
        struct sockaddr_in server_address, client_address;


    public:
        Server() = default;
        Server(unsigned short port);
        void write(std::vector<Byte> to_send);
        std::vector<Byte> recieve();
        void accepting();
        ~Server();

        bool accepted = false;
};

class Client
{
    private:
        unsigned short portno;
        int server_socket;
        int err;
    public:
        Client() = default;
        Client(unsigned short port);
        void write(std::vector<Byte> to_send);
        std::vector<Byte> recieve();
        ~Client();
};

#elif defined(PLATFORM_WINDOWS)

class Server
{
    private:
        unsigned short portno;
        SOCKET server_socket, new_socket;
        int err;
        socklen_t client_len;
        struct sockaddr_in server_address, client_address;


    public:
        Server() = default;
        Server(unsigned short port);
        void write(std::vector<Byte> to_send);
        std::vector<Byte> recieve();
        void accepting();
        ~Server();

        bool accepted = false;
};

class Client
{
    private:
        unsigned short portno;
        SOCKET server_socket;
        int err;
    public:
        Client() = default;
        Client(unsigned short port);
        void write(std::vector<Byte> to_send);
        std::vector<Byte> recieve();
        ~Client();
};

#endif

#endif