//
// Created by cahenk on 4/17/16.
//

#include "Node.hpp"

unsigned int Node::sequenceID = 1;

void Node::sendPacket(const Packet & packet, const unsigned int &tick) {
    if( packet.getDestination().size() == 1){
        this->routingTable[*packet.getDestination().begin()]->receivePacket(packet, tick);
    }
    else{
        std::multimap<Node*, unsigned short> data;

        // Place destinations in the map
        for( auto &d : packet.getDestination())
            data.emplace(this->routingTable[d], d);

        Packet temp = packet;

        // Send packets
        for( auto &n : data){
            // Send modified version of the packet with only 1 destination
            temp.setDestination(n.second);
            // Route the packet
            n.first->receivePacket(temp, tick);
        }
    }
}

void Node::buildRoutes() {

}

// Needed to create array of <Node> , 0 is reserved to single blank node
Node::Node() { ++(Node::sequenceID); }

Node::Node(unsigned short uniqueID)
        : uniqueID{uniqueID}
        { }

void Node::setNeighbors(std::set<Node *> &neighbors) {
    this->neighbors = neighbors;
}

// Note: hopDestID is the MAC ID the message was intended to be sent to
void Node::receivePacket(const Packet &packet, const unsigned int &tick) {
    if(packet.isHighPriority()) {
        if( this->queueDelayTick < 0 || this->queueDelayTick > tick)
            this->queueDelayTick = tick;
    }
    else{
        if( this->queueDelayTick < 0 || this->queueDelayTick > tick + Node::MAX_DELAY_FOR_LOW_PRIORITY)
            this->queueDelayTick = tick + Node::MAX_DELAY_FOR_LOW_PRIORITY;
    }

    this->inputBuffer.push(packet);

    if(this->lastTickActed < tick)
        this->queueCount++;
}

void Node::queuePacket(const Packet &p, const unsigned int & tick) {
    if(p.isHighPriority()) {
        if( this->queueDelayTick < 0 || this->queueDelayTick > tick)
            this->queueDelayTick = tick;
    }
    else{
        if( this->queueDelayTick < 0 || this->queueDelayTick > tick + Node::MAX_DELAY_FOR_LOW_PRIORITY)
            this->queueDelayTick = tick + Node::MAX_DELAY_FOR_LOW_PRIORITY;
    }

    this->outputBuffer.push_back(p);

    if(this->lastSuccessfulRTSTick != 0)
        this->outQueueCount++;
}

void Node::slotAction(const unsigned int &tick, std::queue<Packet> & transmittedPackets) {
    ////////////////// READ //////////////////

    Packet temp;
    while( this->inputBuffer.size() - this->queueCount > 0 ) {
        Packet temp = this->inputBuffer.pop();

        if(temp.findAndRemove(this->uniqueID))
            transmittedPackets.push(temp);

        if(temp.getDestination().size() > 0)
            this->queuePacket(temp, tick);
    }

    ////////////////// SEND + SCHEDULE //////////////////

    if( this->canSend ){
        while(this->outputBuffer.size() - this->outQueueCount > 0) {
            this->sendPacket(this->outputBuffer.front(), tick);
            this->outputBuffer.erase(this->outputBuffer.begin());
        }

        this->queueDelayTick = -1;
        this->lastSuccessfulRTSTick = 0;
        this->outQueueCount = 0;
    }
    else if( this->backoffCounter == 0 ){
        if( this->queueDelayTick < 0 && this->queueDelayTick < tick ) {
            std::set<unsigned short> temp;

            // Union all recipients of the buffer
            for( auto itr = this->outputBuffer.begin(); itr + this->outQueueCount < this->outputBuffer.end(); itr++)
                std::set_union(
                        temp.begin(),
                        temp.end(),
                        (*itr).getDestination().begin(),
                        (*itr).getDestination().end(),
                        temp
                );

            this->emitRTS(this->uniqueID, temp);
        }
    }
    else{
        this->backoffCounter--;
    }

    this->lastTickActed = tick;
}
