#include "network.h"

Server::Server(unsigned short port){
    portno = port;

    // set up non-blocking so if read result in an empty buffer it does not wait for a packet
    // int flags = guard(fcntl(listen_socket_fd, F_GETFL), "could not get flags on TCP listening socket");
    // guard(fcntl(listen_socket_fd, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");

    socklen_t client_len;
    struct sockaddr_in server_address, client_address;
    

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        std::cout << "ERROR opening socket" << std::endl;
    }
        
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);

    // remove naples algorithm
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        std::cout << "ERROR on binding" << std::endl;
    }
              

    listen(server_socket,5);

    client_len = sizeof(client_address);
    new_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_len);
    if (new_socket < 0){
        std::cout << "ERROR on accept" << std::endl;
    }
}

void Server::send(std::vector<Byte> to_send){

    for ( int i = 0; i < 10; i++){
        err = write(new_socket, &to_send[0], to_send.size());
        if (err < 0){
            std::cout << "ERROR writing to socket" << std::endl;
        } 
    }
}

void Server::recieve(){
    std::vector<Byte> buffer(256, 0);

    err = read(new_socket, &buffer[0], buffer.size());

    if (err < 0){
        std::cout << "ERROR reading from socket" << std::endl;
    } 
    else{
        buffer.resize(err);
        std::cout << "Here is the message:" << std::endl;
        for (Byte i: buffer){
            i.print();
            std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

Server::~Server(){
    shutdown(new_socket, SHUT_RDWR);
    shutdown(server_socket, SHUT_RDWR); 
}


Client::Client(unsigned short port){

    portno = port;

    struct sockaddr_in server_address;
    struct hostent *server;
   
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        std::cout << "ERROR opening socket" << std::endl;
    }

    server = gethostbyname("localhost");

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &server_address, sizeof(server_address));

    // setup server address details
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    // add port to server ddress
    server_address.sin_port = htons(portno);

    if (connect(server_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) {
        std::cout << "ERROR connecting" << std::endl;
    }
    else {
        std::cout << " CONECTED TO CLIENT" << std::endl;
    }

}

void Client::recieve(){
    std::vector<float> buffer(256, 0);

    err = read(server_socket, &buffer[0], buffer.size());
    if (err < 0) {
        std::cout << "ERROR reading from socket" << std::endl;
    }
    else{
        buffer.resize(err);
        std::cout << "Here is the returned result" << std::endl;
        for (float i : buffer){
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }
}

void Client::send(std::vector<Byte> to_send){


    err = write(server_socket, &to_send[0], to_send.size());
    if (err < 0) {
        std::cout << "ERROR writing to socket" << std::endl;
    }
}

Client::~Client(){
    shutdown(server_socket, SHUT_RDWR);
}

