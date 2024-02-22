#ifndef COMMUNICATOR 
#define COMMUNICATOR

#include "communicationStruct.h"
#include <string>
#include <thread>
#include "../utils/utils.h"
#include <sys/ioctl.h>
#include <sys/un.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


namespace Communication {
    
    
    
    class Client{
        private:
        
        protected:
            int socketFileDescriptor;
            void waitForMessage(int socketFileDescriptor, char* resultBuffer, unsigned long size, int flags);

        public:
            Client();
            void sendMessage(PacketUnion comm);
            PacketUnion waitForNextMessage();
            void closeSocket();
    };

    Client* initClient();

    class TCPClient : public Client{
        private:
            std::string ip;
            std::string port;

        public:
            TCPClient(std::string ip, std::string port);
    };

    class UnixClient : public Client{
        private:
            std::string address;

        public:
            UnixClient(std::string address);
    };

    enum UserState{
        LOGIN_SCREEN,
        COMMAND_SCREEN,
        IN_GAME,
    };

    class UserInteractionHandler{
        private:
            Client* client;
            bool shouldLoopRun = true;
            bool serverClosedConnection = false;
            bool waitingForServerResponse = false;
            UserState state = LOGIN_SCREEN;
            


            void listenForServerMessages();
            void handleIncommingPacket(CommunicationPacket& packet);
            void handleInput(std::string& userInput);
            void processCommand(std::string& userInput);

        public:
            UserInteractionHandler(Client* client);
            void startLoop();


    };
}

#endif