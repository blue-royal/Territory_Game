#include "network.h"

Networking::Networking(){
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    if (!server_socket) {
        std::cout << "server failed to initialse" << std::endl;
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cout << "server failed to set option" << std::endl;
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_socket,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        std::cout << "server failed to bind" << std::endl;
    }

    // convert the socket to listen for incoming connections
    if (listen(server_socket, SOMAXCONN) < 0) {
        std::cout << "server failed to listen" << std::endl;
    }
}