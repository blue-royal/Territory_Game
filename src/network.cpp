#include "network.h"


#if defined(PLATFORM_UNIX) || defined(PLATFORM_MAC)

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

Server::~Server(){
    // shutdown(new_socket, SHUT_RDWR);
    // shutdown(server_socket, SHUT_RDWR); 
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

Client::~Client(){
    //shutdown(server_socket, SHUT_RDWR);
}

#elif defined(PLATFORM_WINDOWS)

Server::Server(unsigned short port){
    portno = port;

    WSADATA wsaData;

    // Using MAKEWORD macro, Winsock version request 2.2
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        printf("The Winsock dll not found!\n");
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        std::cout << "ERROR SERVER opening socket" << std::endl;
    }


    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);

    if (bind(server_socket, (SOCKADDR*) &server_address, sizeof(server_address)) < 0) {
        std::cout << "ERROR  SERVER on binding" << std::endl;
    }

    Server::accepting();

}

void Server::accepting(){
    listen(server_socket,5);

    client_len = sizeof(client_address);
    new_socket = accept(server_socket, (SOCKADDR*) &client_address, &client_len);
    if (new_socket < 0){
        std::cout << "ERROR SERVER on accept" << std::endl;
    } else{
        accepted = true;
        std::cout << " SERVER accepted" << std::endl;
    }
}

Server::~Server(){

}

Client::Client(unsigned short port){

    portno = port;

    WSADATA wsaData;

    // Using MAKEWORD macro, Winsock version request 2.2
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        printf("The Winsock dll not found!\n");
    }

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

    if (connect(server_socket,(SOCKADDR*) &server_address,sizeof(server_address)) < 0) {
        std::cout << "ERROR CLIENT connecting" << std::endl;
    }
    else {
        std::cout << " CONECTED TO SERVER" << std::endl;
    }

}

Client::~Client(){
}

#endif

void Server::write(std::vector<Byte> to_send){
    err = send(new_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0){
        std::cout << "ERROR SERVER writing to socket" << std::endl;
    } 
}

std::vector<Byte> Server::recieve(){
    std::vector<Byte> buffer(512, 0);

    err = recv(new_socket, &buffer[0], buffer.size(), MSG_DONTWAIT);

    if (err < 0){
        std::cout << "ERROR SERVER reading from socket" << std::endl;
    } 
    else{
        buffer.resize(err);
    }

    return buffer;
}

std::vector<Byte> Client::recieve(){
    std::vector<Byte> buffer(512, 0);

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

    return buffer;
}

void Client::write(std::vector<Byte> to_send){
    unsigned int counter = 0;
    for (Byte i: to_send){
        counter++;
    }
    if (counter == 0){
        std::cout << "DEBUG CLIENT nothing sent" << std::endl;
    }

    err = send(server_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0) {
        std::cout << "ERROR CLIENT writing to socket" << std::endl;
    }

}