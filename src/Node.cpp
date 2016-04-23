//
// Created by cahenk on 4/17/16.
//
#include <algorithm>
#include <climits>

#include "Node.hpp"
#include "Simulator.hpp"

unsigned short Node::sequenceID = 1;

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

// Needed to create array of <Node> , 0 is reserved to single blank node
Node::Node() { this->uniqueID = ++(Node::sequenceID); }

Node::Node(unsigned short uniqueID)
        : uniqueID{uniqueID}
        { }

void Node::setNeighbors(std::set<Node *> &neighbors) {
    this->neighbors = neighbors;
}

void Node::receivePacket(const Packet &packet, const unsigned int &tick) {
    if (packet.isHighPriority()) {
        if (this->queueDelayTick < 0 || this->queueDelayTick > tick)
            this->queueDelayTick = tick;
    }
    else {
        if (this->queueDelayTick < 0 || this->queueDelayTick > tick + Node::MAX_DELAY_FOR_LOW_PRIORITY)
            this->queueDelayTick = tick + Node::MAX_DELAY_FOR_LOW_PRIORITY;
    }
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
        temp = this->inputBuffer.front();
        this->inputBuffer.pop();

        if(temp.findAndRemove(this->uniqueID))
            transmittedPackets.push(temp);

        if(temp.getDestination().size() > 0)
            this->queuePacket(temp, tick);
    }

    ////////////////// SEND + SCHEDULE //////////////////

    if( this->sourceIDCTS != 0 ) {
        this->emitCTS(this->uniqueID, tick);
        this->sourceIDCTS = 0;
    }
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
            std::set<unsigned short> tempset;

            // Union all recipients of the buffer
            for( auto itr = this->outputBuffer.begin(); itr + this->outQueueCount < this->outputBuffer.end(); itr++)
                std::set_union(
                        tempset.begin(),
                        tempset.end(),
                        (*itr).getDestination().begin(),
                        (*itr).getDestination().end(),
                        tempset.begin()
                );

            this->emitRTS(this->uniqueID, tempset);
        }
    }
    else{
        this->backoffCounter--;
    }

    this->lastTickActed = tick;
}

void Node::emitCTS(unsigned short sourceID, const unsigned int &tick) {
    //call receive cts on all neighbors
    for(auto &n : neighbors) {
        n->receiveCTS(sourceID, tick);
    }
}

void Node::emitRTS(unsigned short sourceID, std::set<unsigned short> destinationID) {
    //call receive rts on all neighbors
    for(auto &n : neighbors) {
        n->receiveRTS(sourceID, destinationID);
    }
}

void Node::receiveCTS(unsigned short rstSourceID, const unsigned int &tick) {
    if(rstSourceID == uniqueID) {
        canSend = true;
        lastSuccessfulRTSTick = tick -1;
    }
    else {
        backoffCounter++;
    }
}

void Node::receiveRTS(unsigned short sourceID, std::set<unsigned short> destinationID) {
    if(!collision) {
        if(sourceIDRTS == 0) {
            if(destinationID.find(uniqueID) != destinationID.end()) {
                sourceIDRTS = sourceID;
            }
            else {
                backoffCounter = rand() % 2u << expCounter;
                if(expCounter < 10)
                    expCounter++;
            }
        }
    }
    else {
        collision = true;
        sourceIDRTS = 0;
    }
}

//written by Eric Smith
void Node::buildRoutes() {
    std::vector<Node*> allNodes = buildTopology();

    //initialize routingTable
    for(int i=0;i<allNodes.size();i++)
        routingTable.insert({allNodes.at(i), ULONG_MAX});

    routingTable.at(this) = 0; //set this as root node

    std::vector<Node*> pathKnown; //list of nodes whose least cost path is known
    pathKnown.push_back(this);

    //for all nodes adjacent to root (this), set their distances to 1
    for(int i=0;i<allNodes.size();i++){
        Node* v = allNodes.at(i);

        //current node is adjacent to root (this)
        if( std::find(this->neighbors.begin(), this->neighbors.end(), v) != this->neighbors.end() )
            routingTable.at(v) = 1;
    }

    //loop until all least cost paths are known
    while( pathKnown.size() != allNodes.size() ){
        Node* w;
        unsigned int leastCost = ULONG_MAX;

        //find a node not in pathKnown with the minimum current cost
        for(int i=0;i<allNodes.size();i++){
            int curValue = routingTable.at( allNodes.at(i) );

            if( (std::find(pathKnown.begin(), pathKnown.end(), allNodes.at(i)) == pathKnown.end()) && curValue < leastCost ){
                w = allNodes.at(i);
                leastCost = curValue;
            }
        }

        //add w to list of known shortest paths
        pathKnown.push_back(w);

        //update cost of path for all v adjacent to w and not in pathKnown
        for(auto itr = w->neighbors.begin(); itr != w->neighbors.end(); itr++){
            if( std::find(pathKnown.begin(), pathKnown.end(), *itr) == pathKnown.end() ){
                routingTable.at(*itr) = std::min(routingTable.at(*itr), (routingTable.at(w) + 1));
            }
        }
    }

}

//BFS to find all the nodes in the network
std::vector<Node*>& Node::buildTopology(){
    Queue<Node*> frontier;

    std::vector<Node*> allNodes;

    //mark root visited
    allNodes.push_back(this);
    frontier.push(this);

    while( !frontier.getQueue().empty() ){
        Node* n = frontier.pop();

        for(auto itr = n->neighbors.begin(); itr != n->neighbors.end(); itr++){
            //allNodes doesn't contain nPrime (nPrime hasn't been visited)
            if(std::find(allNodes.begin(), allNodes.end(), *itr) == allNodes.end()){
                frontier.push(*itr);

                //mark nPrime visited
                allNodes.push_back(*itr);
            }
        }
    }
}

void Node::printRoutingTable(){
    std::cout << " ---- Routing Table ----" << std::endl;
    std::cout << "| Unique ID | Distance |" << std::endl;
    std::cout << "------------------------" << std::endl;
    for(auto& x: routingTable){
        if(x.first == this)
            std::cout << "root" << " | " << x.second << std::endl;
        else
            std::cout << x.first->getUniqueID() << " | " << x.second << std::endl;
    }
    std::cout << "------------------------" << std::endl;
}
