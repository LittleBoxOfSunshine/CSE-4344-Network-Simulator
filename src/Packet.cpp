//
// Created by cahenk on 4/17/16.
//

#include "Packet.hpp"

Packet::Packet() {
    this->source = 0;
    this->destination = 0;
    this->highPriority = false;
    this->groupID = 0;
}

Packet::Packet(unsigned long source, unsigned long destination, uint8_t* message, bool highPriority,
               unsigned short groupID)
        : source{source}
        , destination{destination}
        , message{message}
        , highPriority{highPriority}
        , groupID{0}
    {}

uint8_t* Packet::getMessage() { return this->message; }

unsigned long Packet::getSource() { return this->source; }

unsigned long Packet::getDestination() { return this->destination; }

bool Packet::getPriority() { return this->highPriority; }

unsigned short Packet::getGroupID() { return this->groupID; }

bool Packet::isHighPriority() { return this->highPriority; }

bool Packet::isLowPriority() { return !this->highPriority; }

void Packet::setMessage(uint8_t* message) { this->message = message; }

void Packet::setSource(unsigned long source) { this->source = source; }

void Packet::setDestination(unsigned long destination) { this->destination = destination; }

void Packet::setPriority(bool highPriority) { this->highPriority = highPriority; }

void Packet::setHighPriority() { this->highPriority = true; }

void Packet::setLowPriority() { this->highPriority = false; }

void Packet::setGroupID(unsigned short groupID) { this->groupID = groupID; }

Packet& Packet::operator*(const int &fieldConstant) {
    this->source *= fieldConstant;

    // Destination is unmodified by the encoding process

    for(int i = 0; i < Packet::MESSAGE_SIZE; i++)
        this->message[i] *= fieldConstant;
}

Packet& Packet::operator+(const Packet & rhs) {
    this->source += rhs.source;

    // Destination is unmodified by the encoding process

    for(int i = 0; i < Packet::MESSAGE_SIZE; i++)
        this->message[i] += rhs.message[i];

    // Allow higher priority to override lower priority
    this->highPriority |= rhs.highPriority;
}

bool operator<(const Packet &lhs, const Packet &rhs) {
    return lhs.highPriority < rhs.highPriority;
}
