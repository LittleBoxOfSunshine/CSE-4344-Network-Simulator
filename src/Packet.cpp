//
// Created by cahenk on 4/17/16.
//

#include "Packet.hpp"

Packet::Packet() {
    this->source = 0;
    this->highPriority = false;
}

Packet::Packet(unsigned int uniqueID, unsigned long source, unsigned long destination, bool highPriority)
        : uniqueID{uniqueID}
        , source{source}
        , destination{destination}
        , highPriority{highPriority}
    {}

unsigned long Packet::getSource() { return this->source; }

std::set<unsigned long> Packet::getDestination() { return this->destination; }

bool Packet::getPriority() const { return this->highPriority; }

unsigned int Packet::getUniqueID() { return this->uniqueID; }

bool Packet::isHighPriority() { return this->highPriority; }

bool Packet::isLowPriority() { return !this->highPriority; }

void Packet::setUniqueID(unsigned int uniqueID) { this->uniqueID = uniqueID; }

void Packet::setSource(unsigned long source) { this->source = source; }

void Packet::setDestination(std::set<unsigned long> destination) { this->destination = destination; }

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
        return lhs.getPriority() < rhs.getPriority();
    }
};
