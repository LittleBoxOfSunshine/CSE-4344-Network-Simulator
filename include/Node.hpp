//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_NODE_HPP
#define SIMULATOR_NODE_HPP

#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <iostream>
#include <algorithm>

#include "Packet.hpp"

class Simualtor;

class Node {
private:

    //const static unordered_map<unsigned short, std::vector<unsigned int>> GROUP_TABLE;

    static unsigned int sequenceID;
    unsigned int uniqueID;
    unsigned short groupID;
    std::set<Node*> neighbors;
    //std::vector<Packet> packetCache; Would be used for opportunistic opportunities
    std::queue<Packet> inputBuffer;
    std::priority_queue<Packet> outputBuffer;
    std::unordered_map<unsigned int, Node*> routingTable;
    Packet readyToSend;
    bool readyToSendPacketExists = false;

    unsigned int queueCount; // The number of packets that have been added to the queue during this tick
                             // This is needed to prevent receiving and processing a packet in the same tick
                             // It will always be set to 0 at the end of Node::slotAction() and will only be incremented
                             // by Node::receivePacket() and only if Node::lastTickActed < tick

    unsigned int lastTickActed; // Last tick that the node acted on, always updated by Node::slotAction()

    void sendPacket(const Packet & packet, std::set<unsigned int> & macRec, const unsigned int &tick);
    void transmitterAction(); // NOTE: This must check RTS/CTS and should also apply collisions (CSMA/CA)
                                // NOTE: will need some more member variables for this function

public:
    Node();
    Node(unsigned int uniqueID, unsigned short groupID=0);
    void setNeighbors(std::set<Node*> & neighbors);
    void receivePacket(const Packet & packet, const unsigned int & tick); // Called by neighbor nodes when they send a packet
    void queuePacket(const Packet & p); // Called by simulator when a packet is "created" for the node to send
    void slotAction(const unsigned int & tick, std::queue<Packet> & transmittedPackets);
                    // Called by simulator to run the node's actions during the current time slot (tick)
    //void receiveRTS(unsigned int sourceID, unsigned int destinationID);
    //void receiveCTS(unsigned int rstSourceID, unsigned int rstDestinationID);
    void buildRoutes(); // Use Dijkstra's algorithm to build the routing table
};

#endif //SIMULATOR_NODE_HPP
