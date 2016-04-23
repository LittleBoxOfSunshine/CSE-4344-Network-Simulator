//
// Created by cahenk on 4/17/16.
//

#include "Node.hpp"

unsigned int Node::sequenceID = 1;

void Node::sendPacket(const Packet & packet, const unsigned int &tick) {
    /*std::set<Node*> recipients;
    std::set_intersection(
            this->neighbors.begin(),
            this->neighbors.end(),
            macRec.begin(),
            macRec.end(),
            std::back_inserter(recipients)
    );
    */

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
    this->inputBuffer.push(packet);

    if(this->lastTickActed < tick)
        this->queueCount++;
}

void Node::queuePacket(const Packet &p) {
    this->outputBuffer.push(p);
}

void Node::slotAction(const unsigned int &tick, std::queue<Packet> & transmittedPackets) {
    ////////////////// READ //////////////////

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

    ////////////////// SEND + SCHEDULE //////////////////

    if( this->canSend ){

    }
    else if( this->backoffCounter == 0 ){
        if( !this->outputBuffer.empty() ) {
            //this->emitRTS(this->uniqueID, );
        }
    }
    else{
        this->backoffCounter--;
    }

    this->lastTickActed = tick;
}

void Node::emitCTS(unsigned short sourceID, unsigned short destinationID, const unsigned int &tick) {
    //call receive cts on all neighbors
    for(auto &i : neighbors) {
        receiveCTS(sourceID, destinationID, tick);
    }
}

void Node::emitRTS(unsigned short sourceID, std::set<unsigned short> destinationID) {
    //call receive rts on all neighbors
    for(auto &i : neighbors) {
        receiveRTS(sourceID, destinationID);
    }
}

void Node::receiveCTS(unsigned short rstSourceID, unsigned short rstDestinationID, const unsigned int &tick) {
    //if srcID == us
        //can send = true
    //last successfulTick =tick-1
    //else
        //backoffcounter++;
    if(sourceIDCTS == uniqueID) {
        canSend = true;
        lastSuccessfulRTSTick = tick -1;
    }
    else {
        backoffCounter++;
    }
}

void Node::receiveRTS(unsigned short sourceID, std::set<unsigned short> destinationID) {
    //if !collision
        //if sourceIDRTS == 0
            //if destID == us
                //sourceIDRTS = sourceID
            //else
                //backoffcounter = rand(0, 2<<expcounter)
                //expCOunter++;
    //else
        //collision = true
        //sourceIDRTS = 0;
    if(!collision) {
        if(sourceIDRTS == 0) {
            if(destinationID.find(uniqueID) != destinationID.end()) {
                sourceIDRTS = sourceID;
            }
            else {
                backoffCounter = rand() % 0 + 2 << expCounter;//
                expCounter++;
            }
        }
    }
    else {
        collision = true;
        sourceIDRTS = 0;
    }
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