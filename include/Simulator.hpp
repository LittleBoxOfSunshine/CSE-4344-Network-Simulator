//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#ifndef SIMULATOR_SIMULATOR_HPP
#define SIMULATOR_SIMULATOR_HPP

#include "Node.hpp"
#include "Queue.hpp"
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <iterator>
#include <sstream>

class Simulator {
private:
    std::thread simulatorThread;
    signed int sleepTime = 3;
    std::ofstream out;
    Queue<std::string> queue;

    // TODO: member to hold output buffer (queue) used by log thread
    std::vector<Node> nodes;
    unsigned int currentTick;
    std::vector<std::pair<unsigned int, Packet>> unaddedPackets; // Must be presorted
    int packetIndex;

    void runTick(); // Packets that finish transmitting during this tick will be added to Simulator::transmittedPackets

public:
    static std::queue<Packet> transmittedPackets;

    Simulator();
    Simulator(signed int sleepTime);
    Simulator(std::string logFilePath);
    Simulator(signed int sleepTime, std::string logFilePath);
    ~Simulator();

    void handler();
    void log(std::string logString);
    void log(std::vector<std::string> logVector);

    Simulator(std::vector<Node> & nodes, std::vector<std::pair<unsigned int, Packet>> packets);
    void start(); // TODO: Eric


};


#endif //SIMULATOR_SIMULATOR_HPP
