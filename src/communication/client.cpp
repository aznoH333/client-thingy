#include "client.h"



namespace Communication {
    
    
    Client* initTcp(){
        std::cout << "\nserver ip \n";
        std::string ip;
        std::string port;

        std::getline(std::cin, ip);
        std::cout << "\nserver port \n";
        std::getline(std::cin, port);

        return new TCPClient(ip, port);
    }

    Client* initUnix(){
        std::cout << "\nsocket file name \n";
        std::string address;
        std::getline(std::cin, address);
        return new UnixClient("/tmp/" + address + ".sock");

    }
    
    Client* initClient(){
        std::cout << "\nChoose client type\n";
        std::cout << "1) tcp\n2) unix\n";
        
        std::string input;

        std::getline(std::cin, input);

        if (input[0] == '1'){
            return initTcp();
        }else {
            return initUnix();
        }

    }
    void Client::waitForMessage(int socketFileDescriptor, char* resultBuffer, unsigned long size, int flags){
        while (true) {
            int result = recv(socketFileDescriptor, resultBuffer, size, flags);

            if (result == -1){
                perror("recv");
                exit(-1);
            }else if (result != 0){
                break;
            }
        }
    }

    Client::Client(){
    }

    void Client::sendMessage(PacketUnion comm){
        write(socketFileDescriptor, comm.bytes, sizeof(Communication::CommHeader) + comm.packet.header.content.contentSize);
    }

    PacketUnion Client::waitForNextMessage(){
        Communication::PacketUnion result;
        waitForMessage(socketFileDescriptor, result.packet.header.bytes, sizeof(Communication::CommHeader), MSG_PEEK);
        waitForMessage(socketFileDescriptor, result.bytes, result.packet.header.content.contentSize + sizeof(Communication::CommHeader), 0);
        return result;
    }

    void Client::closeSocket(){
        sendMessage(Communication::closeConnection());
        close(socketFileDescriptor);
    }
    
}