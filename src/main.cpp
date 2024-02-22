#include "communication/client.h"


int main()
{
    
    Communication::Client* client = Communication::initClient();
    Communication::UserInteractionHandler handler = Communication::UserInteractionHandler(client);
    
    handler.startLoop();

    delete client;


    return 0;
}