#include "communicationStruct.h"

namespace Communication {

    PacketUnion error(){
        CommunicationPacket out;

        out.header.content.communicationCode = CommunicationCode::ERROR;
        out.header.content.contentSize = 0;
        
        return {out};
    }

    PacketUnion text(std::string text){
        
        CommunicationPacket output;
        output.header.content.communicationCode = CommunicationCode::TEXT;
        output.header.content.contentSize = text.length();


        for (int i = 0; i < (int)text.length(); i++){
            output.content.bytes[i] = text[i];
        }
        
        return {output};
    }

    PacketUnion closeConnection(){
        CommunicationPacket out;

        out.header.content.communicationCode = CommunicationCode::CLOSE_CONNECTION;
        out.header.content.contentSize = 0;
        
        return {out};
    }

    std::string getTextFromContent(CommunicationPacket& packet){
        std::string out = "";

        for (int i = 0; i < packet.header.content.contentSize; i++){
            out += packet.content.bytes[i];
        }

        return out;
    }

    PacketUnion play(int playerId, std::string word){
        CommunicationPacket output;

        output.header.content.communicationCode = CommunicationCode::PLAY;
        output.header.content.contentSize = sizeof(int) + word.length();
        
        // store int as four chars
        Utils::storeIntAsBytes(playerId, output.content.bytes, 0);


        // store text
        for (int i = 0; i < (int) word.length(); i++){
            output.content.bytes[i + 4] = word[i];
        }
        
        
        
        return {output};
    }

    PacketUnion listPlayers(){
        CommunicationPacket out;
        out.header.content.communicationCode = CommunicationCode::LIST_PLAYERS;
        out.header.content.contentSize = 0;
        return {out};
    }

    PacketUnion giveUp(){
        CommunicationPacket out;
        out.header.content.communicationCode = CommunicationCode::GIVE_UP;
        out.header.content.contentSize = 0;
        return {out};
    }


}
