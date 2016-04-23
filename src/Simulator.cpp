//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#include "Simulator.hpp"

std::queue<Packet> Simulator::transmittedPackets;
bool Simulator::simulating = true;

Simulator::Simulator(Node* nodes, int nodeCount, std::vector<std::pair<unsigned int, Packet>> packets)
        : nodes{nodes}
        , nodeCount{nodeCount}
        , unaddedPackets{packets}
{
    //create file at location specified in home dir
    std::string path(getenv("HOME"));
    path += "/log.csv";
    out.open(path);

    //start thread and store it
    simulatorThread = std::thread(&Simulator::handler, this);
}

Simulator::~Simulator(){
    Simulator::simulating = false;
    simulatorThread.join();
    out.close();
}

//print to file if Simulator queue isn't empty
//elsewise sleep for user defined time
void Simulator::handler() {
    while(Simulator::simulating) {
        if (!queue.getQueue().empty()) {
            //print to file
            out << queue.pop() << "\n";
        }
        else {
            sleep(sleepTime);
        }
    }
}

//write string to log.csv
void Simulator::log(std::string logString){
    queue.push(logString);
}

//write vector to log.csv, separating values with comma
void Simulator::log(std::vector<std::string> logVector){
    std::ostringstream logString;

    std::copy(logVector.begin(), logVector.end() - 1, std::ostream_iterator<std::string>(logString, ", "));
    logString << logVector.back();

    queue.push(logString.str());
}

void Simulator::runTick() {
    // Queue packets in nodes to simulate them being "created" for the node to send when it can
    while(this->unaddedPackets[this->packetIndex].first <= this->currentTick) {
        this->nodes[this->unaddedPackets[this->packetIndex].second.getSource()-1]
                .queuePacket(this->unaddedPackets[this->packetIndex].second, this->currentTick);
        packetIndex++;
    }

    // Have all nodes act
    for(int i = 0; i < this->nodeCount; i++)
        this->nodes[i].slotAction(this->currentTick, Simulator::transmittedPackets);
}

void Simulator::start() {
    // Build routing tables on all nodes
    for(int i = 0; i < this->nodeCount; i++)
        this->nodes[i].buildRoutes();

    // TODO: Eric
}