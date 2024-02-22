#include "client.h"

namespace Communication{
    
    // get sockaddr, IPv4 or IPv6:
    void *get_in_addr(struct sockaddr *sa)
    {
        if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
        }

        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }
    
    // --== tcp client ==--
    TCPClient::TCPClient(std::string ip, std::string port): Client(){
        this->ip = ip;
        this->port = port;
        

        struct addrinfo hints, *servinfo, *p;
        int rv;
        char s[INET6_ADDRSTRLEN];

        

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        if ((rv = getaddrinfo(ip.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }

        // loop through all the results and connect to the first we can
        for(p = servinfo; p != NULL; p = p->ai_next) {
            if ((socketFileDescriptor = socket(p->ai_family, p->ai_socktype,
                    p->ai_protocol)) == -1) {
                perror("socket");
                continue;
            }

            if (connect(socketFileDescriptor, p->ai_addr, p->ai_addrlen) == -1) {
                close(socketFileDescriptor);
                perror("connect");
                continue;
            }

            break;
        }

        if (p == NULL) {
            fprintf(stderr, "failed to connect\n");
            exit(2);
        }

        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                s, sizeof(s));
        printf("connecting to %s\n", s);

        freeaddrinfo(servinfo);
    }
    
}