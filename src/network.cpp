#include "network.h"

Server::Server(){
    int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     int portno = 6843;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        std::cout << "ERROR opening socket" << std::endl;
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              std::cout << "ERROR on binding" << std::endl;
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          std::cout << "ERROR on accept" << std::endl;
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) std::cout << "ERROR reading from socket" << std::endl;
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) std::cout << "ERROR writing to socket" << std::endl;
     shutdown(newsockfd, SHUT_RDWR);
     shutdown(sockfd, SHUT_RDWR); 
}


Client::Client(){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
   
    portno = 6843;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        std::cout << "ERROR opening socket" << std::endl;
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        std::cout << "ERROR connecting" << std::endl;
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         std::cout << "ERROR writing to socket" << std::endl;;
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         std::cout << "ERROR reading from socket" << std::endl;;
    printf("%s\n",buffer);
    shutdown(sockfd, SHUT_RDWR);
}