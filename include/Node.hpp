//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_NODE_HPP
#define SIMULATOR_NODE_HPP

#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <cstdint>

#include "Packet.hpp"

class Simualtor;
struct PacketComparison;

class Node {
private:

    const static int MAX_DELAY_FOR_LOW_PRIORITY = 10;

    bool collision = false;
    unsigned short sourceIDRTS = 0;
    unsigned short sourceIDCTS = 0;
    unsigned int backoffCounter;
    uint8_t expCounter;
    bool canSend = false;
    unsigned int outQueueCount;
    unsigned int lastSuccessfulRTSTick;
    int queueDelayTick = 0;
    int alternateDelayTick = 0;

    static unsigned short sequenceID;
    unsigned short uniqueID;
    std::set<Node*> neighbors;
    std::queue<Packet> inputBuffer;
    std::vector<Packet> outputBuffer;
    std::unordered_map<unsigned short, Node*> routingTable;

    unsigned int queueCount; // The number of packets that have been added to the queue during this tick
                             // This is needed to prevent receiving and processing a packet in the same tick
                             // It will always be set to 0 at the end of Node::slotAction() and will only be incremented
                             // by Node::receivePacket() and only if Node::lastTickActed < tick

    unsigned int lastTickActed; // Last tick that the node acted on, always updated by Node::slotAction()

    void sendPacket(const Packet & packet, const unsigned int &tick);
    void emitCTS(unsigned short sourceID, const unsigned int & tick);
    void emitRTS(unsigned short sourceID, std::set<unsigned short> destinationID);
    std::vector<Node*> buildTopology(); // fills allNodes to create topology of network

public:
    Node();
    Node(unsigned short uniqueID);

    void setNeighbors(std::set<Node*> & neighbors);
    void receivePacket(const Packet & packet, const unsigned int & tick); // Called by neighbor nodes when they send a packet
    void queuePacket(const Packet & p, const unsigned int & tick); // Called by simulator when a packet is "created" for the node to send
    void slotAction(const unsigned int & tick, std::queue<Packet> & transmittedPackets);

    // Called by simulator to run the node's actions during the current time slot (tick)
    void receiveRTS(unsigned short sourceID, std::set<unsigned short> destinationID);
    void receiveCTS(unsigned short rstSourceID, const unsigned int & tick);
    void buildRoutes(); // Use Dijkstra's algorithm to build the routing table
    void printRoutingTable();
};

#endif //SIMULATOR_NODE_HPP
