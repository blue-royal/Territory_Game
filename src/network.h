#ifndef __NET__
#define __NET__

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

#include <thread>
#include<chrono>
#include "serialise.h"


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

#endif