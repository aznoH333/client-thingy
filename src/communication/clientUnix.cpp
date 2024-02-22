#include "client.h"

namespace Communication{
    // --== Unix client ==--
    UnixClient::UnixClient(std::string address): Client(){
        this->address = address;

        socketFileDescriptor = socket(AF_UNIX, SOCK_STREAM, 0);
        if (socketFileDescriptor == -1){
            perror("socket");
        }
        
        sockaddr_un addr;
        memset(&addr, 0, sizeof(sockaddr_un));

        /* Connect socket to socket address */

        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, address.c_str(), sizeof(addr.sun_path) - 1);

        int ret = connect (socketFileDescriptor, (sockaddr *) &addr,
                    sizeof(sockaddr_un));
        if (ret == -1) {
            fprintf(stderr, "The server is down.\n");
            exit(EXIT_FAILURE);
        }
        

        
        std::cout << "\nclient connected \n";
        
    }
    
    
    
}