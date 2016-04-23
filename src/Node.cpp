//
// Created by cahenk on 4/17/16.
//
#include <algorithm>
#include <climits>

#include "Node.hpp"
#include "Simulator.hpp"

unsigned int Node::sequenceID = 1;

void Node::sendPacket(const Packet & packet, const int &tick) {
    // TODO: Implement this function
    // TODO: Implement some kind of MAC, probably CSMA/CA (to avoid collisions)
    // Due to broadcast nature of wireless, all neighbors receive the packet
    for( auto &n : this->neighbors)
        n->receivePacket(packet, tick);
}

//written by Eric Smith
void Node::buildRoutes() {
    buildTopology();

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
        if( std::find(this->getNeighbors().begin(), this->getNeighbors().end(), v) != this->getNeighbors().end() )
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
        for(int i=0;i<w->getNeighbors().size();i++){
            Node* v = w->getNeighbors().at(i);

            if( std::find(pathKnown.begin(), pathKnown.end(), v) == pathKnown.end() ){
                routingTable.at(v) = std::min(routingTable.at(v), (routingTable.at(w) + 1));
            }
        }
    }

}

//BFS to find all the nodes in the network
void Node::buildTopology(){
    Queue<Node*> frontier;

    //mark root visited
    allNodes.push_back(this);
    frontier.push(this);

    while( !frontier.getQueue().empty() ){
        Node* n = frontier.pop();

        for(int i=0;i<n->getNeighbors().size();i++){
            Node* nPrime = n->getNeighbors().at(i);

            //allNodes doesn't contain nPrime (nPrime hasn't been visited)
            if(std::find(allNodes.begin(), allNodes.end(), nPrime) == allNodes.end()){
                frontier.push(nPrime);

                //mark nPrime visited
                allNodes.push_back(nPrime);
            }
        }
    }

}


// Needed to create vector<Node>
Node::Node() { this->uniqueID = ++(Node::sequenceID); }

Node::Node(unsigned int uniqueID) : uniqueID{uniqueID} { }

void Node::setNeighbors(std::vector<Node *> &neighbors) {
    this->neighbors = neighbors;
}

std::vector<Node*> & Node::getNeighbors(){
    return this->neighbors;
}

unsigned int Node::getUniqueID(){
    return this->uniqueID;
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

}

Packet* Node::processorAction() {
    // Can Decode 1 packet, or encode 1 packet
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
