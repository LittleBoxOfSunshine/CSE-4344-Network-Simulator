//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_PACKET_HPP
#define SIMULATOR_PACKET_HPP

#include <cstdint>

class Packet {
private:

    uint8_t* message; // Byte array containing the packet's message
    int source; // Unique ID of the source
    int destination; // Unique ID of the source
    bool highPriority; // Signals message must be sent ASAP (true) or it can be delayed for linear combination (false)

public:

    Packet();
    Packet(int source, int destination, uint8_t* message, bool highPriority=false);

    uint8_t* getMessage();
    int getSource();
    int getDestination();
    bool getPriority();

    bool isHighPriority();
    bool isLowPriority();

    void setMessage(uint8_t* message);
    void setSource(int source);
    void setDestination(int destination);
    void setPriority(bool priority);

    void setHighPriority();
    void setLowPriority();

    Packet& operator*(const int & fieldConstant); // To multiply by Galois Field constant
    Packet& operator+(const Packet & rhs); // To add to other packets during linear combinations
    friend bool operator<(const Packet & lhs, const Packet & rhs);
};


#endif //SIMULATOR_PACKET_HPP
