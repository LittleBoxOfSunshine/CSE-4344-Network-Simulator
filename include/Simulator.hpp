//
// Created by cahenk on 4/17/16.
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
#include <array>

class Simulator {
private:
    static bool simulating;
    std::thread simulatorThread;
    unsigned int sleepTime = 3;
    unsigned int numDestinations;
    std::ofstream out;
    Queue<std::string> queue;

    Node* nodes;
    int nodeCount;
    unsigned int currentTick;
    std::vector<Packet> unaddedPackets; // Must be presorted
    int packetIndex = 0;

    void runTick(); // Packets that finish transmitting during this tick will be added to Simulator::transmittedPackets

public:
    Simulator(Node* nodes, int nodeCount, std::vector<Packet> & packets);
    static std::queue<Packet> transmittedPackets;

    ~Simulator();

    void handler();
    void log(std::string logString);
    void log(std::vector<std::string> logVector);

    void start(); // TODO: Eric

};


#endif //SIMULATOR_SIMULATOR_HPP
