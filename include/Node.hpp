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

    int uniqueID;
    std::vector<Node*> neighbors;
    std::vector<Packet> packetCache;
    std::priority_queue<Packet> inputBuffer;
    std::priority_queue<Packet> outputBuffer;
    std::unordered_map<int, Node*> routingTable;

    void sendPacket(Packet packet); //
    void buildRoutes(); // Use Dijkstra's algorithm to build the routing table

public:
    Node(int uniqueID, std::vector<Node*> & neighbors);
    void receivePacket(Packet & packet); // Called by neighbor nodes when they send a packet
    void queuePacket(Packet & p); // Called by simulator when a packet is "created" for the node to send

};


#endif //SIMULATOR_NODE_HPP
