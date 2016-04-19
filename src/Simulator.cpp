//
// Created by cahenk on 4/17/16.
//

#include "Simulator.hpp"

void Simulator::runTick() {
    // Queue packets in nodes to simulate them being "created" for the node to send when it can
    while(this->unaddedPackets[this->packetIndex].first <= this->currentTick) {
        this->nodes[this->unaddedPackets[this->packetIndex].second.getSource()-1]
                .queuePacket(this->unaddedPackets[this->packetIndex].second);
        packetIndex++;
    }

    // Have all nodes act
    for(auto &n : this->nodes)
        n.slotAction(this->currentTick);
}