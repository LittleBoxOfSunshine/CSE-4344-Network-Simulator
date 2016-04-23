//
// Created by cahenk on 4/17/16.
//

#include "Packet.hpp"

Packet::Packet() {
    this->source = 0;
    this->highPriority = false;
}

Packet::Packet(unsigned int uniqueID, unsigned short source, unsigned short destination, unsigned int creationTick,
               bool highPriority)
        : uniqueID{uniqueID}
        , source{source}
        , destination{destination}
        , creationTick{creationTick}
        , highPriority{highPriority}
    {}

unsigned short Packet::getSource() { return this->source; }

const std::set<unsigned short>& Packet::getDestination() const { return this->destination; }

bool Packet::getPriority() const { return this->highPriority; }

unsigned int Packet::getUniqueID() { return this->uniqueID; }

unsigned int Packet::getCreationTick() const { return this->creationTick; }

bool Packet::isHighPriority() { return this->highPriority; }

bool Packet::isLowPriority() { return !this->highPriority; }

void Packet::setUniqueID(unsigned int uniqueID) { this->uniqueID = uniqueID; }

void Packet::setSource(unsigned short source) { this->source = source; }

void Packet::setDestination(std::set<unsigned short> destination) { this->destination = destination; }

void Packet::setDestination(unsigned short destination) { this->destination = {destination}; }

void Packet::setPriority(bool highPriority) { this->highPriority = highPriority; }

void Packet::setHighPriority() { this->highPriority = true; }

void Packet::setLowPriority() { this->highPriority = false; }

bool Packet::operator<(const Packet &rhs) {
    return this->source < rhs.source;
}

bool Packet::operator==(const Packet &rhs) {
    return this->source == rhs.source;
}

struct PacketComparison{
    bool operator()(const Packet &lhs, const Packet &rhs) {
        if( lhs.getPriority() == rhs.getPriority() )
            return lhs.getCreationTick() < rhs.getCreationTick();
        else
            return lhs.getPriority() < rhs.getPriority();
    }
};
