#include "network.h"

Server::Server(unsigned short port){
    portno = port;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        std::cout << "ERROR SERVER opening socket" << std::endl;
    }


    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        std::cout << "ERROR  SERVER on binding" << std::endl;
    }

    Server::accepting();

}

void Server::accepting(){
    listen(server_socket,5);

    client_len = sizeof(client_address);
    new_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_len);
    if (new_socket < 0){
        std::cout << "ERROR SERVER on accept" << std::endl;
    } else{
        accepted = true;
        std::cout << " SERVER accepted" << std::endl;
    }
}

void Server::write(std::vector<Byte> to_send){
    unsigned int counter = 0;
    for (Byte i: to_send){
        i.print();
        std::cout << ", ";
        counter++;
    }
    std::cout << std::endl;
    if (counter == 0){
        std::cout << "DEBUG SERVER nothing sent" << std::endl;
    }

    std::cout << "server sending" << std::endl;
    err = send(new_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0){
        std::cout << "ERROR SERVER writing to socket" << std::endl;
    } 
    std::cout << "server sent" << std::endl;
}

std::vector<Byte> Server::recieve(){
    std::cout << "server recieving" << std::endl;
    std::vector<Byte> buffer(256, 0);

    err = recv(new_socket, &buffer[0], buffer.size(), MSG_DONTWAIT);

    if (err < 0){
        std::cout << "ERROR SERVER reading from socket" << std::endl;
    } 
    else{
        buffer.resize(err);
    }
    std::cout << "server recieved" << std::endl;

    return buffer;
}

Server::~Server(){
    // shutdown(new_socket, SHUT_RDWR);
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
        std::cout << "ERROR CLIENT connecting" << std::endl;
    }
    else {
        std::cout << " CONECTED TO SERVER" << std::endl;
    }

}

std::vector<Byte> Client::recieve(){
    // std::cout << "client recieving" << std::endl;
    std::vector<Byte> buffer(256, 0);

    err = recv(server_socket, &buffer[0], buffer.size(), MSG_DONTWAIT);
    if (errno == EWOULDBLOCK){
        std::cout << "ERROR CLIENT did not recieve any packets" << std::endl;
    }
    if (err < 0){
        std::cout << "ERROR CLIENT reading from socket" << std::endl;
    }
    else{
        buffer.resize(err);
    }

    unsigned int counter = 0;
    std::cout << "client recieved" << std::endl;
    for(Byte i: buffer){
        i.print();
        std::cout << ", ";
        counter ++;
    }
    std::cout << std::endl;

    if (counter == 0){
        std::cout << "DEBUG CLIENT nothing recieved" << std::endl;
    }

    return buffer;
}

void Client::write(std::vector<Byte> to_send){
    std::cout << "client sending" << std::endl;
    unsigned int counter = 0;
    for (Byte i: to_send){
        i.print();
        std::cout << ", ";
        counter++;
    }
    std::cout << std::endl;
    if (counter == 0){
        std::cout << "DEBUG CLIENT nothing sent" << std::endl;
    }

    err = send(server_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0) {
        std::cout << "ERROR CLIENT writing to socket" << std::endl;
    }
    std::cout << "client sent" << std::endl;

}

Client::~Client(){
    //shutdown(server_socket, SHUT_RDWR);
}

