//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#include "Simulator.hpp"

//create and write log.csv to Desktop
Simulator::Simulator(){
    std::string path(getenv("HOME"));
    out.open(path);
    simulatorThread = std::thread(&Simulator::handler, this);
}

//set thread sleep time
//create and write log.csv to Desktop
Simulator::Simulator(signed int sleepTime):
Simulator(){
    this->sleepTime = sleepTime;
}

//create and write log.csv to user defined path
Simulator::Simulator(std::string logFilePath){
    logFilePath += "log.csv";
    out.open(logFilePath);

    if( !out.is_open() )
        std::cout << "Error creating/writing to specified path for Simulator" << std::endl;

    simulatorThread = std::thread(&Simulator::handler, this);
}

//set thread sleep time
//create and write log.csv to Desktop
Simulator::Simulator(signed int sleepTime, std::string logFilePath):
Simulator(logFilePath){
    this->sleepTime = sleepTime;
}

Simulator::~Simulator(){
    simulatorThread.join();
    out.close();
}

//print to file if Simulator queue isn't empty
//elsewise sleep for user defined time
void Simulator::handler() {
    if( !queue.getQueue().empty() ){
        //print to file
        out << queue.pop() << "\n";
    }
    else{
        sleep(sleepTime);
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
                .queuePacket(this->unaddedPackets[this->packetIndex].second);
        packetIndex++;
    }

    // Have all nodes act
    for(auto &n : this->nodes)
        n.slotAction(this->currentTick, Simulator::transmittedPackets);
}