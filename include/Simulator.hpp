//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_SIMULATOR_HPP
#define SIMULATOR_SIMULATOR_HPP

#include "Node.hpp"

class Simulator {

private:

    // TODO: member to hold output buffer (queue) used by log thread
    std::vector<Node> nodes;
    unsigned int currentTick;
    std::vector<std::pair<unsigned int, Packet>> unaddedPackets; // Must be presorted
    int packetIndex;
    static std::vector<Packet> transmittedPackets;

    void runTick(); // Packets that finish transmitting during this tick will be added to Simulator::transmittedPackets

public:

    Simulator(std::vector<Node> & nodes, std::vector<std::pair<unsigned int, Packet>> packets);
    void start(); // TODO: Eric


    // TODO: log function

};


#endif //SIMULATOR_SIMULATOR_HPP
