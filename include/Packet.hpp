//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_PACKET_HPP
#define SIMULATOR_PACKET_HPP

#include <cstdint>
#include <vector>
#include <set>

class Packet {
private:

    unsigned int uniqueID;
    unsigned long source; // Unique ID of the source
    std::set<unsigned long> destination; // Unique ID of the destination
    bool highPriority; // Signals message must be sent ASAP (true) or it can be delayed for linear combination (false)

public:

    Packet();
    Packet(unsigned int uniqueID, unsigned long source, unsigned long destination, bool highPriority=false);

    unsigned int getUniqueID();
    unsigned long getSource();
    std::set<unsigned long> getDestination();
    bool getPriority() const;

    bool isHighPriority();
    bool isLowPriority();

    void setUniqueID(unsigned int uniqueID);
    void setSource(unsigned long source);
    void setDestination(std::set<unsigned long> destination);
    void setPriority(bool priority);

    void setHighPriority();
    void setLowPriority();

    bool operator<(const Packet &rhs);
    bool Packet::operator==(const Packet &rhs);
};


#endif //SIMULATOR_PACKET_HPP
