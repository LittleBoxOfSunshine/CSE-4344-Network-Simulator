//
// Created by cahenk on 4/17/16.
//

#include "Node.hpp"

unsigned int Node::sequenceID = 1;

void Node::sendPacket(const Packet & packet, std::set<unsigned int> & macRec, const unsigned int &tick) {
    // Due to broadcast nature of wireless, all neighbors receive the packet
    std::set<Node*> recipients;
    std::set_intersection(
            this->neighbors.begin(),
            this->neighbors.end(),
            macRec.begin(),
            macRec.end(),
            std::back_inserter(recipients)
    );

    for( auto &n : recipients)
        n->receivePacket(packet, tick);

    //for( auto &n : this->neighbors)
        //if(maceRec.find(n->uniqueID) != macRec.end())
            //n->receivePacket(packet, tick);
}

void Node::buildRoutes() {

}

// Needed to create array of <Node> , 0 is reserved to single blank node
Node::Node() { ++(Node::sequenceID); }

Node::Node(unsigned int uniqueID, unsigned short groupID)
        : uniqueID{uniqueID}
        , groupID{groupID}
        { }

void Node::setNeighbors(std::set<Node *> &neighbors) {
    this->neighbors = neighbors;
}

// Note: hopDestID is the MAC ID the message was intended to be sent to
void Node::receivePacket(const Packet &packet, const unsigned int &tick) {
    this->inputBuffer.push(packet);

    if(this->lastTickActed < tick)
        this->queueCount++;
}

void Node::queuePacket(const Packet &p) {
    this->outputBuffer.push(p);
}

void Node::slotAction(const unsigned int &tick, std::queue<Packet> & transmittedPackets) {
    // TODO: This should ensure that collisions are handled properly
    // TODO: Implement this function

    this->transmitterAction();

    ////////////////// DECODE OPERATION //////////////////

    // Process Packet in input buffer (if one exists)
    if( this->inputBuffer.size() - this->queueCount == 1) {
        // Read and remove packet from queue
        Packet recPacket = this->inputBuffer.front();
        this->inputBuffer.pop();

        std::vector<Packet> decodedPackets;
        // Decode packet if needed (gfmval is the m in GF(2^m) with 0 reserved for no encoding
        if( recPacket.getGfmval() != 0 ){
            // Perform decode operation
            // append each decoded packet to decodedPackets
        }
        else{ // Add packet as sole member of list
            decodedPackets.push_back(recPacket);
        }

        // Iterator over packets
        for( auto &p : decodedPackets) {
            // Is Unicast Destination or a Multicast Destination
            if( this->uniqueID == p.getDestination() || (this->groupID != 0 && this->groupID == p.getGroupID()) ) {
                // Notify simulator that transmission has completed
                transmittedPackets.push(p);
            }
                // Is Other Destination (i.e. to be routed)
            else{
                this->outputBuffer.push(p);
            }
        }

        // Buffer is guaranteed to be "empty" (with respect to current tick) at this point so reset queueCount
        this->queueCount = 0;

    }
    else if(this->inputBuffer.size() - this->queueCount > 1){
        std::cerr << "ERROR: inputBuffer.size() = " << this->inputBuffer.size() << " is invalid. Must be 0 or 1..." << std::endl;
    }

    this->lastTickActed = tick;
}

// Determine if temp packet received and if should be added
void Node::transmitterAction() {

    // Handle tick reception

    // Collision Test (receive 2 or more messages at once). This is all that is needed on receiving (left in queue for processing)
    if( this->inputBuffer.size() - this->queueCount > 2) {
        // Collision, packets are corrupted so flush the buffer
        while ( this->inputBuffer.size() > this->queueCount)
            this->inputBuffer.pop();
            // If we need to debug collisions print that a collision has occurred at this point, list packets lost

        // Buffer is guaranteed to be "empty" (with respect to current tick) at this point so reset queueCount
        this->queueCount = 0;
    }

    // Scheduling (only schedule if there is something waiting to be sent
    if()

}