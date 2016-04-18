//
// Created by cahenk on 4/17/16.
//

#include "Packet.hpp"

Packet::Packet() {
    this->source = -1;
    this->destination = -1;
    this->highPriority = false;
}

Packet::Packet(int source, int destination, uint8_t* message, bool highPriority)
        : source{source}
        , destination{destination}
        , message{message}
        , highPriority{highPriority}
    {}

uint8_t* Packet::getMessage() { return this->message; }

int Packet::getSource() { return this->source; }

int Packet::getDestination() { return this->destination; }

bool Packet::getPriority() { return this->highPriority; }

bool Packet::isHighPriority() { return this->highPriority; }

bool Packet::isLowPriority() { return !this->highPriority; }

void Packet::setMessage(uint8_t* message) { this->message = message; }

void Packet::setSource(int source) { this->source = source; }

void Packet::setDestination(int destination) { this->destination = destination; }

void Packet::setPriority(bool highPriority) { this->highPriority = highPriority; }

void Packet::setHighPriority() { this->highPriority = true; }

void Packet::setLowPriority() { this->highPriority = false; }

Packet& Packet::operator*(const int &fieldConstant) {
    this->source *= fieldConstant;
    // TODO: Determine if destination will be encoded or prepended to messages (more data but prevents uneeded decode)
    this->destination *= fieldConstant;

    for(int i = 0; i < Packet::MESSAGE_SIZE; i++)
        this->message[i] *= fieldConstant;
}

Packet& Packet::operator+(const Packet & rhs) {
    this->source += rhs.destination;
    // TODO: Determine if destination will be encoded or prepended to messages (more data but prevents uneeded decode)
    this->destination += rhs.destination;

    for(int i = 0; i < Packet::MESSAGE_SIZE; i++)
        this->message[i] += rhs.message[i];

    // Allow higher priority to override lower priority
    this->highPriority |= rhs.highPriority;
}

bool operator<(const Packet &lhs, const Packet &rhs) {
    return lhs.highPriority < rhs.highPriority;
}
