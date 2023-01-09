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
    public:
        Server(int port);
};

class Client
{
    public:
        Client(int port);
};

enum class Message_Header : int { draw_message, input_message };

struct Message{
    Message_Header header;
    
    

};


#endif