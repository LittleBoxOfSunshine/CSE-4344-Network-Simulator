//
// Created by cahenk on 4/17/16.
//

#include "Node.hpp"

unsigned int Node::sequenceID = 1;

void Node::sendPacket(const Packet & packet, const int &tick) {
    // Due to broadcast nature of wireless, all neighbors receive the packet
    for( auto &n : this->neighbors)
        n->receivePacket(packet, tick);
}

void Node::buildRoutes() {

}

// Needed to create vector<Node>
Node::Node() { this->uniqueID = ++(Node::sequenceID); }

Node::Node(unsigned int uniqueID) : uniqueID{uniqueID} { }

void Node::setNeighbors(std::vector<Node *> &neighbors) {
    this->neighbors = neighbors;
}

void Node::receivePacket(const Packet &packet, const int &tick) {
    this->inputBuffer.push(packet);

    if(this->lastTickActed < tick)
        this->queueCount++;
}

void Node::queuePacket(const Packet &p) {
    this->outputBuffer.push(p);
}

void Node::slotAction(const int &tick, std::queue<Packet> & transmittedPackets) {
    // TODO: This should ensure that collisions are handled properly
    // TODO: Implement this function

    this->transmitterAction();
    this->processorAction();

    // while(this->inputBuffer.size() > this->queueCount) for packet processing
    // last steps will be => this->queueCount = 0; this->lastTickActed = tick;
}

void Node::emitRTS() {
    for( auto &n : this->neighbors )
        n->receiveRTS();
}

void Node::emitCTS() {
    for( auto &n : this->neighbors )
        n->receiveCTS();
}

void Node::receiveRTS() {
    this->receivedRTS = true;
}

void Node::receiveCTS() {
    this->receivedCTS = true;
}

void Node::transmitterAction() {
    // Determine if temp packet received and if should be added
        // should not be added if rts or cts receive or if multiple received

    // TODO: Implement this function
    // TODO: Implement some kind of MAC, probably CSMA/CA (to avoid collisions)
}

Packet* Node::processorAction() {
    // Can Decode 1 packet, or encode 1 packet

}