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

    //const static unordered_map<> GROUP_TABLE;

    static unsigned int sequenceID;
    unsigned int uniqueID;
    unsigned short groupID;
    std::vector<Node*> neighbors;
    std::vector<Packet> packetCache;
    std::queue<Packet> inputBuffer;
    std::priority_queue<Packet> outputBuffer;
    std::unordered_map<unsigned int, Node*> routingTable;
    bool receivedCTS;
    bool receivedRTS;

    unsigned int queueCount; // The number of packets that have been added to the queue during this tick
                             // This is needed to prevent receiving and processing a packet in the same tick
                             // It will always be set to 0 at the end of Node::slotAction() and will only be incremented
                             // by Node::receivePacket() and only if Node::lastTickActed < tick

    unsigned int lastTickActed; // Last tick that the node acted on, always updated by Node::slotAction()

    void sendPacket(const Packet & packet, const int &tick);
    void buildRoutes(); // Use Dijkstra's algorithm to build the routing table
    void transmitterAction(); // NOTE: This must check RTS/CTS and should also apply collisions (CSMA/CA)
                                // NOTE: will need some more member variables for this function
    void processerAction(); // NOTE: at most 1 encode or decode per tick, this is also where routing happens
                            // TODO: Emit signal when packet has arrived at intended destination
    void emitRTS();
    void emitCTS();


public:
    Node();
    Node(unsigned int uniqueID);
    void setNeighbors(std::vector<Node*> & neighbors);
    void receivePacket(const Packet & packet, const int & tick); // Called by neighbor nodes when they send a packet
    void queuePacket(const Packet & p); // Called by simulator when a packet is "created" for the node to send
    void slotAction(const int & tick); // Called by simulator to run the node's actions during the current time slot (tick)
    void receiveRTS();
    void receiveCTS();
};

#endif //SIMULATOR_NODE_HPP
