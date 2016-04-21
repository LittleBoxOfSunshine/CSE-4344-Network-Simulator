//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_PACKET_HPP
#define SIMULATOR_PACKET_HPP

#include <cstdint>
#include <vector>

class Packet {
private:

    uint8_t* message; // Byte array containing the packet's message
    unsigned long source; // Unique ID of the source
    unsigned long destination; // Unique ID of the destination
    unsigned long lastHopID = 0; // Unique ID of the device that sent
    bool highPriority; // Signals message must be sent ASAP (true) or it can be delayed for linear combination (false)
    unsigned short groupID;
    unsigned int gfmval = 0;

public:

    /// Size of messages in bytes, padding will be added if message is short
    const static uint8_t MESSAGE_SIZE = 102;

    /// Size of 802.15.4 frame in bytes
    const static uint8_t FRAME_SIZE = 127;

    Packet();
    Packet(unsigned long source, unsigned long destination, uint8_t* message, bool highPriority=false,
           unsigned short groupID=0);

    uint8_t* getMessage();
    unsigned long getSource();
    unsigned long getDestination();
    bool getPriority();
    unsigned short getGroupID();
    unsigned int getGfmval();
    unsigned int getLastHopID();

    bool isHighPriority();
    bool isLowPriority();

    void setMessage(uint8_t* message);
    void setSource(unsigned long source);
    void setDestination(unsigned long destination);
    void setPriority(bool priority);
    void setGroupID(unsigned short groupID);
    void setGfmval(unsigned int gfmval);
    void setLastHopID(unsigned int lastHopID);

    void setHighPriority();
    void setLowPriority();

    /// To multiply by Galois Field constant
    Packet& operator*(const int & fieldConstant);

    /// To add to other packets during linear combinations
    Packet& operator+(const Packet & rhs);

    //// To allow for use with std::priority_queue, compares using lhs.highPriority < rhs.highPriority
    friend bool operator<(const Packet & lhs, const Packet & rhs);
};


#endif //SIMULATOR_PACKET_HPP
