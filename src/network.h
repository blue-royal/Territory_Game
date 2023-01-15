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

#include "serialise.h"


class Server
{
    private:
        unsigned short portno;
        int server_socket, new_socket, err;
    public:
        Server(unsigned short port);
        void send(std::vector<Byte> to_send);
        void recieve();
        ~Server();
};

class Client
{
    private:
        unsigned short portno;
        int server_socket, err;

    public:
        Client(unsigned short port);
        void send(std::vector<Byte> to_send);
        void recieve();
        ~Client();
};

#endif