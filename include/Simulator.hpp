//
// Created by cahenk on 4/17/16.
// Written by Eric Smith
//

#ifndef SIMULATOR_SIMULATOR_HPP
#define SIMULATOR_SIMULATOR_HPP

#include "Node.hpp"
#include "Queue.hpp"
#include <string.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <iterator>
#include <sstream>

class Simulator {
private:
    static bool simulating;
    std::thread simulatorThread;
    unsigned int sleepTime = 3;
    std::ofstream out;
    Queue<std::string> queue;

    std::vector<Node> nodes;
    unsigned int currentTick;
    std::vector<std::pair<unsigned int, Packet>> unaddedPackets; // Must be presorted
    int packetIndex;

    void runTick(); // Packets that finish transmitting during this tick will be added to Simulator::transmittedPackets

public:
    Simulator(std::vector<Node> & nodes, std::vector<std::pair<unsigned int, Packet>> packets);
    static std::queue<Packet> transmittedPackets;

    ~Simulator();

    void handler();
    void log(std::string logString);
    void log(std::vector<std::string> logVector);

    void start(); // TODO: Eric

};


#endif //SIMULATOR_SIMULATOR_HPP
