//
// Created by cahenk on 4/17/16.
//

#include "Node.hpp"

void Node::sendPacket(Packet packet, const int &tick) {
    // TODO: Implement some kind of MAC, probably CSMA/CA (to avoid collisions)
    // Due to broadcast nature of wireless, all neighbors receive the packet
    for( auto &n : this->neighbors)
        n->receivePacket(packet, tick);
}

void Node::buildRoutes() {

}

// Needed to create vector<Node>
Node::Node() { }

Node::Node(int uniqueID, std::vector<Node *> &neighbors)
    : uniqueID{uniqueID}
    , neighbors{neighbors}
    { }

void Node::receivePacket(Packet &packet, const int &tick) {
    this->inputBuffer.push(packet);

    if(this->lastTickActed < tick)
        this->queueCount++;
}

void Node::queuePacket(const Packet &p) {
    this->outputBuffer.push(p);
}

void Node::slotAction(int &tick) {
    // while(this->inputBuffer.size() > this->queueCount) for packet processing
    // last steps will be => this->queueCount = 0; this->lastTickActed = tick;
}