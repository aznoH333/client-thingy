#include "client.h"

namespace Communication{
    // --== User Interaction Handler ==--
    UserInteractionHandler::UserInteractionHandler(Client* client){
        this->client = client;
    }

    void UserInteractionHandler::startLoop(){
        shouldLoopRun = true;
        serverClosedConnection = false;

        // start listening thread
        std::thread listenThread(&UserInteractionHandler::listenForServerMessages, this);
        std::string userInput;
        while (shouldLoopRun) {
            if (waitingForServerResponse){
                continue;
            }
            
            std::getline(std::cin, userInput);
            if (serverClosedConnection){
                break; // will crash if removed
            }
            handleInput(userInput);
        }
        // close
        if (!serverClosedConnection){
            client->closeSocket();
        }
        listenThread.join();

    }

    void UserInteractionHandler::listenForServerMessages(){
        while(shouldLoopRun){
            Communication::CommunicationPacket incommingPacket = client->waitForNextMessage().packet;
            handleIncommingPacket(incommingPacket);
        }
    }

    void UserInteractionHandler::handleIncommingPacket(CommunicationPacket& incommingPacket){
        waitingForServerResponse = false;
        
        switch (incommingPacket.header.content.communicationCode) {
            case CommunicationCode::TEXT:
                std::cout << "\n" << getTextFromContent(incommingPacket) << "\n";
                break;
            case CommunicationCode::CLOSE_CONNECTION:
                std::cout << "\nServer closed connection \n";
                serverClosedConnection = true;
                shouldLoopRun = false;
                exit(0);
                break;
            case CommunicationCode::PLAY:
                state = state == COMMAND_SCREEN ? IN_GAME : COMMAND_SCREEN;
                break;
            case CommunicationCode::ERROR: 
            case CommunicationCode::LIST_PLAYERS: // client should never recieve this
            default:
                std::cout << "Error occured, closing connection \n";
                shouldLoopRun = false;
                break;
            
        }
    }

    void UserInteractionHandler::handleInput(std::string& input){
        input = input.substr(0, MAX_CONTENT_SIZE);
        

        switch (state) {
            case LOGIN_SCREEN:
                waitingForServerResponse = true;
                state = COMMAND_SCREEN;
                client->sendMessage(text(input));
                break;
            case COMMAND_SCREEN:
                processCommand(input);
                break;
            case IN_GAME:
                if (input == "give up"){
                    client->sendMessage(giveUp());
                }else {
                    client->sendMessage(text(input));

                }
                break;
        }
    }

    void UserInteractionHandler::processCommand(std::string& input){
        std::vector<std::string> split = Utils::splitString(input, " ");
        
        if (split[0] == "help"){
            std::cout << "\nlist : list all players \n";
            std::cout << "play (player id) (word): play game with player \n";
            std::cout << "quit : exit program \n";


        }else if (split[0] == "list"){
            client->sendMessage(listPlayers());
        }else if (split[0] == "play"){
            try {
                client->sendMessage(play(std::stoi(split[1]), split[2]));
                waitingForServerResponse = true;

            }
            catch (...) {
                std::cout << "invalid arguments\n";
            }
            
            
        }else if (split[0] == "quit"){
            client->sendMessage(closeConnection());
            waitingForServerResponse = true;
        }else {
            std::cout << "\nunknown command \n"; 
        }
    }
}