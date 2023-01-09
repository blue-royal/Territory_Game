#include "network.h"

Server::Server(int port){
    int portno = port;

    // set up non-blocking so if read result in an empty buffer it does not wait for a packet
    // int flags = guard(fcntl(listen_socket_fd, F_GETFL), "could not get flags on TCP listening socket");
    // guard(fcntl(listen_socket_fd, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");


    int server_socket, new_socket;
    socklen_t client_len;
    char buffer[256];
    struct sockaddr_in server_address, client_address;
    int err;
    

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

    bzero(buffer,256);
    err = read(new_socket, buffer, 255);
    if (err < 0){
        std::cout << "ERROR reading from socket" << std::endl;
    } 

    printf("Here is the message: %s\n",buffer);

    err = write(new_socket,"I got your message",18);
    if (err < 0){
        std::cout << "ERROR writing to socket" << std::endl;
    } 

    shutdown(new_socket, SHUT_RDWR);
    shutdown(server_socket, SHUT_RDWR); 
}


Client::Client(int port){

    int portno = port;

    int server_socket, err;
    struct sockaddr_in server_address;
    struct hostent *server;

    char buffer[256];
   
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

    printf("Please enter the message: ");

    // clear the buffer
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    err = write(server_socket,buffer,strlen(buffer));
    if (err < 0) {
        std::cout << "ERROR writing to socket" << std::endl;
    }

    bzero(buffer, 256);
    err = read(server_socket,buffer,255);
    if (err < 0) {
        std::cout << "ERROR reading from socket" << std::endl;
    }

    printf("%s\n",buffer);
    
    shutdown(server_socket, SHUT_RDWR);
}