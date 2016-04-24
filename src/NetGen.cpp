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
    std::set<Node*> leafNeighbor{nodes};
    std::set<Node*> leaves;
    for(int i=1;i<numNodes;i++) {
        nodes[i].setNeighbors(leafNeighbor);
        leaves.insert(&nodes[i]);
    }
    nodes[0].setNeighbors(leaves);
    return nodes;
}

/// Generate a random network with n nodes arranged in the grid topology
Node* gridGen(int numNodes, int numColumns) {
    Node* nodes = new Node[numNodes];
    std::set<Node*> neighbors;
    for(int i=0;i<numNodes;i++){
        neighbors.clear();
        if(i - numColumns >= 0){
            neighbors.insert(&nodes[i-numColumns]);
        }
        if(i + numColumns < numNodes){
            neighbors.insert(&nodes[i+numColumns]);
        }
        if(i%numColumns!=0){
            neighbors.insert(&nodes[i-1]);
        }
        if((i+1)%numColumns!=0 && i+1 < numNodes){
            neighbors.insert(&nodes[i+1]);
        }
        nodes[i].setNeighbors(neighbors);
    }
    return nodes;
}

/// Generate a random network with n nodes arranged in the mesh topology
Node* meshGen(int numNodes, std::vector<std::vector<std::string>>& neighborList) {
    Node* nodes= new Node[numNodes];
    /*for(int i = 1; i < numNodes+1; i++) {     //create nodes with messages and tick time
        nodes.at(i) = new Node(i);
    }*/
    for(int i = 0; i < neighborList.size(); i++) {
        std::set<Node*> neighbors;
        for(int j = 0; j < neighborList[i].size(); j++) {
            neighbors.insert(&nodes[std::stoi(neighborList[i][j])]);
        }
        nodes[i].setNeighbors(neighbors);
    }
    for(int i = 0;i < numNodes; i++) {
        std::cout << nodes[i].uniqueID << ": ";
        for(auto itr = nodes[i].neighbors.begin(); itr != nodes[i].neighbors.end(); itr++) {
            std::cout << (*itr)->uniqueID << " ";
        }
        std::cout << std::endl;
    }
    return nodes;

}

