//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#include "Simulator.hpp"

int Node::MAX_DELAY_FOR_LOW_PRIORITY = 1024;
bool Node::NETWORK_CODING = true;

Simulator::Simulator(Node* nodes, int nodeCount, std::vector<Packet> & packets)
        : nodes{nodes}
        , nodeCount{nodeCount}
        , unaddedPackets{packets}
{
    for(int i = 0;i<packets.size();i++) {
        this->numDestinations+=packets[i].getDestination().size();
    }
    std::cout << this->numDestinations << " <- numDestinations" << std::endl;
    //create file at location specified in home dir
    std::string path(getenv("HOME"));
    path += "/log.csv";
    out.open(path);

    //start thread and store it
    simulatorThread = std::thread(&Simulator::handler, this);
}

Simulator::~Simulator(){
    delete[] this->nodes;
    out.close();
}

//print to file if Simulator queue isn't empty
//elsewise sleep for user defined time
void Simulator::handler() {
    std::cout << "start output thread" << std::endl;
    while(Simulator::simulating || !queue.getQueue().empty()) {
        if (!queue.getQueue().empty()) {
            //print to file
            this->out << queue.pop() << std::endl;
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
    while(this->packetIndex < this->unaddedPackets.size() &&
            this->unaddedPackets[this->packetIndex].getCreationTick() <= this->currentTick) {
     //   std::cout << "Adding packet to queue" <<
       //         this->unaddedPackets[this->packetIndex].getSource() <<
         //       " -> ";
        auto t = this->unaddedPackets[this->packetIndex].getDestination();
        //for(auto itr = t.begin(); itr != t.end(); itr++)
          //  std::cout << *itr << " ";
        //std::cout << std::endl;
        this->nodes[this->unaddedPackets[this->packetIndex].getSource()-1]
                .queuePacket(this->unaddedPackets[this->packetIndex], this->currentTick);
        packetIndex++;
    }

    // Have all nodes act
    for(int i = 0; i < this->nodeCount; i++)
        this->nodes[i].slotAction(this->currentTick, Simulator::transmittedPackets);

    this->currentTick++;
}

void Simulator::start(bool networkCoding) {
    if(!networkCoding) {
        Node::MAX_DELAY_FOR_LOW_PRIORITY = 0;
        Node::NETWORK_CODING = false;
    }

    // Build routing tables on all nodes
    for (int i = 0; i < this->nodeCount; i++)
        this->nodes[i].buildRoutes();

    int numTicksDataArrived=0;
    while (this->numDestinations > 0 ){
        if(this->currentTick%1000 == 0) {
            std::cout << "Starting Tick " << this->currentTick << ", " << numDestinations << " Destinations Remain..." << std::endl;
        }
        this->runTick();
        Packet transmitted;
        unsigned short destinationReached;
        if(!transmittedPackets.empty())
        {
            numTicksDataArrived++;
        }
        while(!transmittedPackets.empty()){
            transmitted = transmittedPackets.front().second;
            destinationReached = transmittedPackets.front().first;
            transmittedPackets.pop();
            this->numDestinations--;
            std::string packetMessage = "Packet #" + std::to_string(transmitted.getUniqueID()) + " has reached destination node " + std::to_string(destinationReached) + " in " + std::to_string(this->currentTick - transmitted.getCreationTick()) + " ticks.\n";
            //std::cout << packetMessage;
            this->log(packetMessage);
        }
    }
    sleep(Simulator::sleepTime*2);
    Simulator::simulating = false;
    simulatorThread.join();
    std::cout << "Simulation completed in " << this->currentTick << " ticks" << std::endl;
}