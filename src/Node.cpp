//
// Created by cahenk on 4/17/16.
//
#include <algorithm>
#include <climits>
#include <stack>

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
//Dijkstra's Algorithm
void Node::buildRoutes() {
    buildTopology();

    std::unordered_map<Node*, std::tuple<unsigned short, int, Node*> >initialRouting;

    //initialize routingTable
    for(int i=0;i<allNodes.size();i++) {
        //destination, # hops, 1st hop
        std::tuple<unsigned short, int, Node*> tempTuple(allNodes.at(i)->getUniqueID(), USHRT_MAX, allNodes.at(i));
        initialRouting.insert({allNodes.at(i),tempTuple});
    }

    initialRouting.at(this) = std::make_tuple(this->getUniqueID(), 0, this); //set this as root node

    std::vector<Node*> pathKnown; //list of nodes whose least cost path is known
    pathKnown.push_back(this);

    //for all nodes adjacent to root (this), set their distances to 1
    for(int i=0;i<allNodes.size();i++){
        Node* v = allNodes.at(i);

        //current node is adjacent to root (this)
        if( std::find(this->getNeighbors().begin(), this->getNeighbors().end(), v) != this->getNeighbors().end() )
            initialRouting.at(v) = std::make_tuple(v->getUniqueID(), 1, v);
    }

    //loop until all least cost paths are known
    while( pathKnown.size() != allNodes.size() ){
        Node* w;
        unsigned int leastCost = USHRT_MAX;

        //find a node not in pathKnown with the minimum current cost
        for(int i=0;i<allNodes.size();i++){
            std::tuple<unsigned short, int, Node*> tempTuple = initialRouting.at( allNodes.at(i) );
            int curValue = std::get<1>(tempTuple);

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
                    while( std::find(this->getNeighbors().begin(), this->getNeighbors().end(), targetFirstHop) == this->getNeighbors().end() ){
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
        routingTable.insert({x.first->getUniqueID(), std::get<2>(tempT)});
    }


}

//BFS to find all the nodes in the network
void Node::buildTopology(){

    std::queue<Node*> frontier;

    //mark root visited
    allNodes.push_back(this);
    frontier.push(this);

    while( !frontier.empty() ){
        for(auto &n : frontier.front()->getNeighbors() ){

            //allNodes doesn't contain nPrime (nPrime hasn't been visited)
            if(std::find(allNodes.begin(), allNodes.end(), n) == allNodes.end()){
                frontier.push(n);

                //mark nPrime visited
                allNodes.push_back(n);
            }
        }

        frontier.pop();
    }

}


// Needed to create vector<Node>
Node::Node() { this->uniqueID = ++(Node::sequenceID); }

Node::Node(unsigned short uniqueID) : uniqueID{uniqueID} { }

void Node::setNeighbors(std::vector<Node *> &neighbors) {
    this->neighbors = neighbors;
}

std::vector<Node*> & Node::getNeighbors(){
    return this->neighbors;
}

unsigned short Node::getUniqueID(){
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
    std::cout << "| Dest ID | First Hop |" << std::endl;
    std::cout << "------------------------" << std::endl;
    for(auto& x: routingTable){
        if(x.first == this->getUniqueID())
            std::cout << "  " << "root" << "\t|\t" << x.second->getUniqueID() << std::endl;
        else
            std::cout << "  " << x.first << "\t\t|\t" << x.second->getUniqueID() << std::endl;
    }
    std::cout << "------------------------" << std::endl;

}
