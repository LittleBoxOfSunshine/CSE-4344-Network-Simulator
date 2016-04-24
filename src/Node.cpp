//
// Created by cahenk on 4/17/16.
//
#include <algorithm>
#include <climits>
#include <stack>

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
                        std::inserter(tempset, tempset.begin())
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
//Dijkstra's Algorithm
void Node::buildRoutes() {
    std::vector<Node*> allNodes = buildTopology();

    std::unordered_map<Node*, std::tuple<unsigned short, unsigned int, Node*> >initialRouting;

    //initialize routingTable
    for(int i=0;i<allNodes.size();i++) {
        //destination, # hops, 1st hop
        std::tuple<unsigned short, unsigned int, Node*> tempTuple(allNodes.at(i)->uniqueID, UINT_MAX, allNodes.at(i));
        initialRouting.insert({allNodes[i],tempTuple});
    }

    initialRouting.at(this) = std::make_tuple(this->uniqueID, 0, this); //set this as root node

    std::vector<Node*> pathKnown; //list of nodes whose least cost path is known
    pathKnown.push_back(this);

    //for all nodes adjacent to root (this), set their distances to 1
    for(int i=0;i<allNodes.size();i++){
        Node* v = allNodes[i];

        //current node is adjacent to root (this)
        if( std::find(this->neighbors.begin(), this->neighbors.end(), v) != this->neighbors.end() )
            initialRouting.at(v) = std::make_tuple(v->uniqueID, 1, v);
    }

    //loop until all least cost paths are known
    while( pathKnown.size() != allNodes.size() ){
        Node* w = nullptr;
        unsigned int leastCost = UINT_MAX;

        //find a node not in pathKnown with the minimum current cost
        for(int i=0;i<allNodes.size();i++){
            std::tuple<unsigned short, unsigned int, Node*> tempTuple = initialRouting.at( allNodes.at(i) );
            unsigned int curValue = std::get<1>(tempTuple);

            if( (std::find(pathKnown.begin(), pathKnown.end(), allNodes[i]) == pathKnown.end()) && curValue < leastCost ){
                w = allNodes[i];
                leastCost = curValue;
            }
        }

        //add w to list of known shortest paths
        pathKnown.push_back(w);

        //update cost of path for all v adjacent to w and not in pathKnown
        for(auto itr = w->neighbors.begin(); itr != w->neighbors.end(); itr++){
            Node* v = *itr;

            if( std::find(pathKnown.begin(), pathKnown.end(), v) == pathKnown.end() ){
                std::tuple<unsigned short, int, Node*> vTuple = initialRouting.at(v);
                int vDist = std::get<1>(vTuple);

                std::tuple<unsigned short, int, Node*> wTuple = initialRouting.at(w);
                int wDist = std::get<1>(wTuple);

                int minDist = std::min(vDist, (wDist + 1));

                Node* firstHop = std::get<2>(vTuple);

                //if this node is beyond depth 1, go back through nodes until firstHop is found
                //firstHop is a neighbor of the root (this)
                if( (wDist + 1) < vDist ){

                    Node* targetFirstHop = w;
                    while( std::find(this->neighbors.begin(), this->neighbors.end(), targetFirstHop) == this->neighbors.end() ){
                        std::tuple<unsigned short, int, Node*> target = initialRouting.at(targetFirstHop);
                        targetFirstHop = std::get<2>(target);
                    }

                    firstHop = targetFirstHop;
                }

                initialRouting.at(v) = std::make_tuple(std::get<0>(vTuple), minDist, firstHop);
            }
        }
    }

    //populate routingTable
    for(auto& x : initialRouting ){
        std::tuple<unsigned short, int, Node*> tempT = x.second;
        //int dist = std::get<1>(tempT); //distance
        routingTable.insert({x.first->uniqueID, std::get<2>(tempT)});
    }


}

//BFS to find all the nodes in the network
std::vector<Node*> Node::buildTopology(){
    std::queue<Node*> frontier;

    std::vector<Node*> allNodes;

    //mark root visited
    allNodes.push_back(this);
    frontier.push(this);

    while( !frontier.empty() ){
        for(auto &n : frontier.front()->neighbors ){
            //allNodes doesn't contain nPrime (nPrime hasn't been visited)
            if(std::find(allNodes.begin(), allNodes.end(), n) == allNodes.end()){
                frontier.push(n);

                //mark nPrime visited
                allNodes.push_back(n);
            }
        }
        frontier.pop();
    }

    return allNodes;
}

void Node::printRoutingTable(){
    std::cout << " ---- Routing Table ----" << std::endl;
    std::cout << "| Dest ID | First Hop |" << std::endl;
    std::cout << "------------------------" << std::endl;
    for(auto& x: routingTable){
        if(x.first == this->uniqueID)
            std::cout << "  " << "root" << "\t|\t" << x.second->uniqueID << std::endl;
        else
            std::cout << "  " << x.first << "\t\t|\t" << x.second->uniqueID << std::endl;
    }
    std::cout << "------------------------" << std::endl;

}
