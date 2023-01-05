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


class Server
{
    private:
    int server_socket;

    public:
        Server();
        // ~Server();
};

class Client
{
    public:
        Client();
};

#endif