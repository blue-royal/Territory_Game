#include "network.h"

// run macos and linux networking drivers
#if defined(PLATFORM_UNIX) || defined(PLATFORM_MAC)

Server::Server(unsigned short port){
    portno = port;

    // set up socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        std::cout << "ERROR SERVER opening socket" << std::endl;
    }

    // set up the address information for the sicket
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);

    // start the socket
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        std::cout << "ERROR  SERVER on binding" << std::endl;
    }

    // offer to accept a client to accept
    Server::accepting();

}

void Server::accepting(){
    listen(server_socket,5);

    client_len = sizeof(client_address);
    // create a new socket that accepts the connection to communicate using
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

    // setup data for connecting to server
    struct sockaddr_in server_address;
    struct hostent *server;
   
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        std::cout << "ERROR opening socket" << std::endl;
    }


    // get the name of the server
    server = gethostbyname("localhost");

    // error message if fails to connect
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

    // connect to server with address that has been found
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

    // setup server socket with tcp enabled
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        std::cout << "ERROR SERVER opening socket" << std::endl;
    }


    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(portno);

    // bind the server to given address
    if (bind(server_socket, (SOCKADDR*) &server_address, sizeof(server_address)) < 0) {
        std::cout << "ERROR  SERVER on binding" << std::endl;
    }

    // accept incoming connections
    Server::accepting();

}

void Server::accepting(){
    // listen for incoming connections
    listen(server_socket,5);

    //accept any connections that have been heard
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

    // setup data structures for
    struct sockaddr_in server_address;
    struct hostent *server;
   
    // create a socket to connect to server using TCP protocol
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        std::cout << "ERROR opening socket" << std::endl;
    }

    //  get the address of the server
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
    // send array of bytes and select relevant memory locations
    err = send(new_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0){
        std::cout << "ERROR SERVER writing to socket" << std::endl;
    } 
}

std::vector<Byte> Server::recieve(){
    // set up array of bytes to put incoming data
    std::vector<Byte> buffer(512, 0);

    err = recv(new_socket, &buffer[0], buffer.size(), MSG_DONTWAIT);

    // error catching if no data is recieced
    if (err < 0){
        std::cout << "ERROR SERVER reading from socket" << std::endl;
    } 
    else{
        buffer.resize(err);
    }

    return buffer;
}

std::vector<Byte> Client::recieve(){
    // set up empty array of bytes
    std::vector<Byte> buffer(512, 0);

    // recieve incoming data
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

    // return the vector of bytes
    return buffer;
}

void Client::write(std::vector<Byte> to_send){
    unsigned int counter = 0;
    // find the size of bytes
    for (Byte i: to_send){
        counter++;
    }
    // error message if there is no data in vector
    if (counter == 0){
        std::cout << "DEBUG CLIENT nothing sent" << std::endl;
    }

    // send the data from the byte vector
    err = send(server_socket, &to_send[0], to_send.size(), MSG_DONTWAIT);
    if (err < 0) {
        std::cout << "ERROR CLIENT writing to socket" << std::endl;
    }

}