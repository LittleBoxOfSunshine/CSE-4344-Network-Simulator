//
// Created by cahenk on 4/17/16.
//

/// This file contains code for random network / topology generation

#include <vector>
#include <string>
#include <vector>
#include <iostream>

#include "matrix.hpp"
#include "Node.hpp"


/// Generate a random network with n nodes arranged in the star topology
std::vector<Node*> starGen(int numNodes) {
    std::vector<Node*> nodes(numNodes,new Node());
    Node* center = nodes[0];
    nodes.push_back(center);
    for(int i=1;i<numNodes;i++) {
        Node* leaf = nodes[i];
        std::vector<Node*> leafNeighbor;
        leafNeighbor.push_back(center);
        leaf->setNeighbors(leafNeighbor);
        nodes.push_back(leaf);
    }
    std::vector<Node*> leaves(nodes.begin()+1,nodes.end());
    center->setNeighbors(leaves);
    return nodes;
}

/// Generate a random network with n nodes arranged in the grid topology
std::vector<Node*> gridGen(int numNodes, int numColumns) {
    std::vector<Node*> nodes(numNodes,new Node());
    for(int i=0;i<numNodes;i++){
        std::vector<Node*> neighbors;
        if(i - numColumns >= 0){
            neighbors.push_back(nodes[i-numColumns]);
        }
        if(i + numColumns < numNodes){
            neighbors.push_back(nodes[i+numColumns]);
        }
        if(i%numColumns!=0){
            neighbors.push_back(nodes[i-1]);
        }
        if((i+1)%numColumns!=0 && i+1 < numNodes){
            neighbors.push_back(nodes[i+1]);
        }
        nodes[i]->setNeighbors(neighbors);
    }
    return nodes;
}

/// Generate a random network with n nodes arranged in the mesh topology
std::vector<Node*> meshGen(int numNodes, std::vector<std::vector<std::string>>& neighborList) {
    std::vector<Node*> nodes(numNodes+1);
    for(int i = 1; i < nodes.size(); i++) {     //create nodes with messages and tick time
        nodes.at(i) = new Node(i);
    }
    for(int i = 1; i < neighborList.size(); i++) {
        std::vector<Node*> neighbors;
        for(int j = 0; j < neighborList.at(i).size(); j++) {
            neighbors.push_back(nodes.at(std::stoi(neighborList.at(i).at(j))));
        }
        nodes.at(i)->setNeighbors(neighbors);
    }
    return nodes;

}

