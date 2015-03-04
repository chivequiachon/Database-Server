#include "packet_creator.h"

#include <cstring>
#include <iostream>

std::string PacketCreator::formPacket(const std::string header,
									  const std::string message,
									  const int requestType,
									  ChecksumModule *c)
{
	// packet
	std::string packet = "NULL";
    if( message.length() > 9 ||
        header.length() > 4 ||
        header.length() < 4)
        return packet;

	// temporary packet
	unsigned char tmp_packet[16] = "";

    // append header to packet
    int pos = 0;
    for(unsigned int i = 0; i < header.length(); i++, pos++)
        tmp_packet[pos] = static_cast<unsigned char>(header[i]);

    // append message to packet
    for(unsigned int i = 0; i < message.length(); i++, pos++)
        tmp_packet[pos] = static_cast<unsigned char>(message[i]);
    
    // replace null terminators
    if(message.length() < 9)
    {
        int pos = header.length() + message.length();
        for(; pos < 13; pos++) tmp_packet[pos] = '_';
    }

    tmp_packet[13] = static_cast<unsigned char>(requestType);
    tmp_packet[14] = c->calculateChecksum(tmp_packet, 14);
    tmp_packet[15] = '\0';

    packet = std::string(reinterpret_cast<char*>(tmp_packet));
    
    return packet;
}

std::string PacketCreator::formReadPacket(std::string header,
                                          unsigned char *id,
                                          int requestType,
                                          ChecksumModule *c)
{
    // packet
    std::string packet = "NULL";
    
    unsigned int idSize = std::char_traits<char>::length(reinterpret_cast<char*>(id));
    
    if(idSize != 4)
        return packet;
    
    // temporary packet
    unsigned char tmp_packet[11] = "";

    // append header to packet
    int pos = 0;
    for(unsigned int i = 0; i < header.length(); i++, pos++)
        tmp_packet[pos] = static_cast<unsigned char>(header[i]);

    // append id to packet
    for(unsigned int i = 0; i < idSize; i++, pos++)
        tmp_packet[pos] = id[i];

    tmp_packet[8] = static_cast<unsigned char>(requestType);

    tmp_packet[9] = c->calculateChecksum(tmp_packet, 9);
    tmp_packet[10] = '\0';

    packet = std::string(reinterpret_cast<char*>(tmp_packet));

    return packet;
}

std::string PacketCreator::formWritePacket(std::string header,
                                           unsigned char *id,
                                           unsigned char *message,
                                           int requestType,
                                           ChecksumModule *c)
{
    // packet
    std::string packet = "NULL";
    
    unsigned int idSize = std::char_traits<char>::length(reinterpret_cast<char*>(id));
    unsigned int messageSize = std::char_traits<char>::length(reinterpret_cast<char*>(message));
    
    if(idSize != 4 || messageSize > 4)
        return packet;
    
    // temporary packet
    unsigned char tmp_packet[15] = "";

    // append header to packet
    int pos = 0;
    for(unsigned int i = 0; i < header.length(); i++, pos++)
        tmp_packet[pos] = static_cast<unsigned char>(header[i]);

    // append id to packet
    for(unsigned int i = 0; i < idSize; i++, pos++)
        tmp_packet[pos] = id[i];

    tmp_packet[8] = static_cast<unsigned char>(requestType);
    pos++;

    // append message to packet
    for(unsigned int i = 0; i < messageSize; i++, pos++)
        tmp_packet[pos] = message[i];
    
    // replace null terminators
    if(messageSize < 4)
    {
        int pos = header.length() + idSize + messageSize + 1;
        for(; pos < 13; pos++) tmp_packet[pos] = '_';
    }

    tmp_packet[13] = c->calculateChecksum(tmp_packet, 13);
    tmp_packet[14] = '\0';

    packet = std::string(reinterpret_cast<char*>(tmp_packet));
    
    return packet;
}