//
// Created by cahenk on 4/17/16.
//

#ifndef SIMULATOR_NODE_HPP
#define SIMULATOR_NODE_HPP

#include <vector>
#include <unordered_map>
#include <queue>

#include "Packet.hpp"

class Node {
private:

    unsigned int uniqueID;
    std::vector<Node*> neighbors;
    std::vector<Packet> packetCache;
    std::priority_queue<Packet> inputBuffer;
    std::priority_queue<Packet> outputBuffer;
    std::unordered_map<unsigned int, Node*> routingTable;

    unsigned int queueCount; // The number of packets that have been added to the queue during this tick
                             // This is needed to prevent receiving and processing a packet in the same tick
                             // It will always be set to 0 at the end of Node::slotAction() and will only be incremented
                             // by Node::receivePacket() and only if Node::lastTickActed < tick

    unsigned int lastTickActed; // Last tick that the node acted on, always updated by Node::slotAction()

    void sendPacket(const Packet & packet, const int &tick);
    void buildRoutes(); // Use Dijkstra's algorithm to build the routing table

public:
    Node();
    Node(int uniqueID, std::vector<Node*> & neighbors);
    void receivePacket(Packet & packet, const int & tick); // Called by neighbor nodes when they send a packet
    void queuePacket(const Packet & p); // Called by simulator when a packet is "created" for the node to send
    void slotAction(const int & tick); // Called by simulator to run the node's actions during the current time slot (tick)
};


#endif //SIMULATOR_NODE_HPP
