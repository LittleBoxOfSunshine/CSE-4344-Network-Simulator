//
// Created by cahenk on 4/17/16.
//

/// This file contains code for random network / topology generation

#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "matrix.hpp"
#include "Node.hpp"


/// Generate a random network with n nodes arranged in the star topology
Node* starGen(int numNodes) {
    Node* nodes = new Node[numNodes];
    Node center = nodes[0];
    for(int i=1;i<numNodes;i++) {
        Node leaf = nodes[i];
        std::set<Node> leafNeighbor;
        leafNeighbor.insert(center);
        leaf.setNeighbors(leafNeighbor);
    }
    std::vector<Node> leaves(nodes+1,nodes+numNodes);
    center.setNeighbors(leaves);
    return nodes;
}

/// Generate a random network with n nodes arranged in the grid topology
Node* gridGen(int numNodes, int numColumns) {
    Node* nodes = new Node[numNodes];
    for(int i=0;i<numNodes;i++){
        std::set<Node> neighbors;
        if(i - numColumns >= 0){
            neighbors.insert(nodes[i-numColumns]);
        }
        if(i + numColumns < numNodes){
            neighbors.insert(nodes[i+numColumns]);
        }
        if(i%numColumns!=0){
            neighbors.insert(nodes[i-1]);
        }
        if((i+1)%numColumns!=0 && i+1 < numNodes){
            neighbors.insert(nodes[i+1]);
        }
        nodes[i].setNeighbors(neighbors);
    }
    return nodes;
}

/// Generate a random network with n nodes arranged in the mesh topology
Node* meshGen(int numNodes, std::vector<std::vector<std::string>>& neighborList) {
    Node* nodes= new Node[numNodes+1];
    /*for(int i = 1; i < numNodes+1; i++) {     //create nodes with messages and tick time
        nodes.at(i) = new Node(i);
    }*/
    for(int i = 1; i < neighborList.size(); i++) {
        std::set<Node> neighbors;
        for(int j = 0; j < neighborList.at(i).size(); j++) {
            neighbors.insert(nodes[std::stoi(neighborList.at(i).at(j))]);
        }
        nodes[i].setNeighbors(neighbors);
    }
    return nodes;

}

